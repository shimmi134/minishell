/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/16 21:11:45 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "minishell.h"

int ft_lensplit(char **split){
	int	i;

	i = 0;
	while (split && split[i])
		i++;
	return i;
}


int	do_builtins(t_shell *elem, t_env **env)
{
	if (!ft_strncmp(elem->command->cmd, "pwd", 3))
		return(do_pwd());
	else if (!ft_strncmp(elem->command->cmd, "env", 3))
		return (do_env(env));
	else if (!ft_strncmp(elem->command->cmd, "unset", 5))
		return (do_unset(elem, env));
	else if (!ft_strncmp(elem->command->cmd, "echo", 4))
		return (do_echo(elem, env));
	else if (!ft_strncmp(elem->command->cmd, "export", 6))
		return (do_export(elem,env));
	else if (!ft_strncmp(elem->command->cmd, "cd", 2))
		return do_cd(elem, env);
	return (0);
}

void free_penv(char **penv)
{
	int i;

	i = 0;

	if (!penv)
		return ;
	while (penv[i])
		free(penv[i++]);
	free(penv);
}

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


static void set_out_flags(int *flags, int append)
{
    *flags = 0;
    if (append)
    {
        *flags = O_WRONLY | O_CREAT | O_APPEND;
        return;
    }
    *flags = O_WRONLY | O_CREAT | O_TRUNC;
}

static void open_and_dup_outfile(const char *path, int append)
{
    int flags;
    int fd;
    int res;
    flags = 0;
    fd = -1;
    res = 0;
    set_out_flags(&flags, append);
    fd = open(path, flags, 0644);
    if (fd < 0)
    {
        perror(path);
        exit(1);
    }
    res = dup2(fd, STDOUT_FILENO);
    if (res < 0)
    {
        perror(path);
        exit(1);
    }
    close(fd);
}

static int run_standalone_builtin(t_shell *elem, t_env **env)
{
    int code;
    code = 0;
    if (elem->command->outfile)
        open_and_dup_outfile(elem->command->outfile,
                             elem->command->append);
    code = do_builtins(elem, env);
    if (elem->exit_status_code)
        *(elem->exit_status_code) = code;
    return code;
}

static void prepare_pipe_if_needed(int next_pipe[2], int need)
{
    int res;
    res = 0;
    next_pipe[0] = -1;
    next_pipe[1] = -1;
    if (need)
    {
        res = pipe(next_pipe);
        if (res < 0)
        {
            perror("pipe");
            exit(1);
        }
    }
}

static void dup_prev_to_stdin(int prev_fd)
{
    int res;
    res = 0;
    if (prev_fd != -1)
    {
        res = dup2(prev_fd, STDIN_FILENO);
        if (res < 0)
        {
            perror("dup2");
            exit(1);
        }
    }
}

static void dup_next_to_stdout(int next_write)
{
    int res;
    res = 0;
    if (next_write != -1)
    {
        res = dup2(next_write, STDOUT_FILENO);
        if (res < 0)
        {
            perror("dup2");
            exit(1);
        }
    }
}

static void child_close_unneeded_fds(int prev_fd, int next_read, int next_write)
{
    if (prev_fd != -1) close(prev_fd);
    if (next_read != -1) close(next_read);
    if (next_write != -1) close(next_write);
}

static void child_handle_infile(t_shell *elem)
{
    int fd;
    int res;
    fd = -1;
    res = 0;
    if (elem->command->infile)
    {
        fd = open(elem->command->infile, O_RDONLY);
        if (fd < 0)
        {
            perror(elem->command->infile);
            exit(1);
        }
        res = dup2(fd, STDIN_FILENO);
        if (res < 0)
        {
            perror(elem->command->infile);
            exit(1);
        }
        close(fd);
    }
}

static void child_handle_outfile(t_shell *elem)
{
    int fd;
    int flags;
    fd = -1;
    flags = 0;
    if (elem->command->outfile)
    {
        set_out_flags(&flags, elem->command->append);
        fd = open(elem->command->outfile, flags, 0644);
        if (fd < 0)
        {
            perror(elem->command->outfile);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror(elem->command->outfile);
            exit(1);
        }
        close(fd);
    }
}

static void child_exec_or_builtin(t_shell *elem, t_env **env)
{
    char **penv;
    penv = NULL;
    if (elem->command->cmd != NULL)
    {
        penv = create_envp(*env);
        exec_command(elem, env, penv);
        perror("exec");
        exit(1);
    }
    else
    {
        int code;
        code = do_builtins(elem, env);
        exit(code);
    }
}

static void child_process(t_shell *elem, t_env **env, int prev_fd,
                          int next_write)
{
    dup_prev_to_stdin(prev_fd);
    dup_next_to_stdout(next_write);
    child_close_unneeded_fds(prev_fd, -1, next_write);
    child_handle_infile(elem);
    child_handle_outfile(elem);
    child_exec_or_builtin(elem, env);
}

static void parent_housekeeping(int *prev_fd, int next_read, int next_write)
{
    if (*prev_fd != -1)
    {
        close(*prev_fd);
    }
    if (next_write != -1)
    {
        close(next_write);
    }
    if (next_read != -1)
    {
        *prev_fd = next_read;
    }
    else
    {
        *prev_fd = -1;
    }
}

static pid_t spawn_one_process(t_shell *elem, t_env **env, int *prev_fd)
{
    int next_pipe[2];
    pid_t pid;
    int need_next;
    int next_read;
    int next_write;
    next_read = -1;
    next_write = -1;
    next_pipe[0] = -1;
    next_pipe[1] = -1;
    need_next = 0;
    if (elem->next != NULL) need_next = 1;
    prepare_pipe_if_needed(next_pipe, need_next);
    if (need_next) next_read = next_pipe[0];
    if (need_next) next_write = next_pipe[1];
    pid = fork();
    if (pid < 0) perror("fork");
    if (pid < 0) exit(1);
    if (pid == 0)
    {
        if (next_read != -1) close(next_read);
        child_process(elem, env, *prev_fd, next_write);
        exit(1);
    }
    parent_housekeeping(prev_fd, next_read, next_write);
    return pid;
}


static void wait_children_and_set_status(int *pids, int count,
                                         int *last_status_ptr)
{
    int i;
    int status;
    i = 0;
    status = 0;
    while (i < count)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
        {
            if (last_status_ptr)
            {
                *last_status_ptr = WEXITSTATUS(status);
            }
        }
        else
        {
            if (last_status_ptr)
            {
                *last_status_ptr = 127;
            }
        }
        i = i + 1;
    }
}

/* execute_loop does the per-element loop; <= 25 lines */
static int execute_loop(t_shell *elem, t_env **env, int *pids,
                        int **last_status_ptr_out)
{
    int prev_fd, count;
    prev_fd = -1;
    count = 0;
    while (elem && elem->type)
    {
        if (strncmp(elem->type, "built-in", 9) == 0 && elem->next == NULL)
        {
            run_standalone_builtin(elem, env);
            *last_status_ptr_out = elem->exit_status_code;
            elem = elem->next;
            continue;
        }
        pid_t pid;
        pid = spawn_one_process(elem, env, &prev_fd);
        pids[count] = (int)pid;
        count = count + 1;
        *last_status_ptr_out = elem->exit_status_code;
        elem = elem->next;
    }
    if (prev_fd != -1) close(prev_fd);
    return count;
}


void do_commands(t_shell *elem, t_env **env, int ac)
{
    int *pids;
    int old_stdout;
    int *last_status_ptr;
    int count;
    pids = NULL;
    old_stdout = -1;
    last_status_ptr = NULL;
    count = 0;
    pids = calloc(ac + 1, sizeof(int));
    old_stdout = dup(STDOUT_FILENO);
    count = execute_loop(elem, env, pids, &last_status_ptr);
    wait_children_and_set_status(pids, count, last_status_ptr);
    if (old_stdout != -1) dup2(old_stdout, STDOUT_FILENO);
    if (old_stdout != -1) close(old_stdout);
    free(pids);
}



int	env_len(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

void do_shlvl(t_env *env)
{
	char *str;

		if (ft_atoi(env->value) >= 1000)
			env->value = ft_itoa(1);
		str = env->value;
		env->value = ft_itoa(ft_atoi(env->value) + 1);
		free(str);
}

char *create_pair(t_env *env)
{
	char *temp;
	char *str;

	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	str = ft_strjoin(temp, env->value);
	free(temp);
	if (!str)
		return (NULL);
	return (str);
}

char **create_envp(t_env *env)
{
	char	**penv;
	int		i;
	int		len;
	
	i = 0;
	len = env_len(env);
	penv = malloc(sizeof(char *) * (len + 1));
	if (!penv)
		return (NULL);
	while (i < len && env)
	{
		if (!ft_strncmp(env->key, "SHLVL=", 6))
			do_shlvl(env);
		penv[i] = create_pair(env);
		env = env->next;
		i++;
	}

	penv[i] = NULL;
	return (penv);
}

void create_shlvl(t_env *tail)
{
    t_env *new_node;

	new_node = create_node("SHLVL=2");
	tail->next = new_node;
	new_node->next = NULL;

}

void init_env_vals(t_env **head, int *i, int *shlvl)
{
	*head = NULL;
	*i = 0;
	*shlvl = 0;
}

t_env *copy_env(char *envp[])
{
    t_env *head;
    t_env *tail;
    t_env *new_node;
    int i;
	int shlvl;

	init_env_vals(&head, &i, &shlvl);
    while (envp[i])
    {
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			shlvl = 1;
        new_node = create_node(envp[i]);
        if (!new_node)
            return (NULL);
        if (!head)
            head = new_node;
        else
            tail->next = new_node;
        tail = new_node;
        i++;
    }
	if (!shlvl)
		create_shlvl(tail);
    return (head);
}

void	free_split(char **sp)
{
	int	i;

	i = 0;
	while (sp && sp[i])
	{
		free(sp[i]);
		i++;
	}
	if (sp)
		free(sp);
}

char	*get_element(char *line)
{
	char	**split;

	split = ft_split(line, ' '); //Free this split
	if (split && (!ft_strncmp(split[0], "pwd", 4) || !ft_strncmp(split[0], "env", 4)
			|| !ft_strncmp(split[0], "unset", 6) || !ft_strncmp(split[0],
				"echo", 5) || !ft_strncmp(split[0], "export", 6)
			|| !ft_strncmp(split[0], "cd", 2)))
	{
		return (free_split(split), "built-in");
	}
	else
	{
		return (free_split(split), "command");
	}
}

void do_struct(t_shell **element, t_cmd *command, int *exit_status)
{
    t_shell *new_node;
    t_shell *last;

    *element = NULL;
	last = NULL;
    while (command)
    {
        new_node = malloc(sizeof(t_shell));
        if (!new_node)
            exit(1);
        new_node->command = command;
        new_node->type = get_element(command->cmd);
		new_node->exit_status_code = exit_status;
        new_node->next = NULL;
        if (!*element)
            *element = new_node;
        if (last)
            last->next = new_node;
        last = new_node;
        command = command->next;
    }
}

void free_shell(t_shell *element)
{
	t_shell *temp;

	while (element)
	{
		temp = element;
		element = element->next;
		free(temp);
	}
}

int if_valid(char *str)
{
	int len;

	len = ft_strlen(str);
	if (len == 3) 
	{
		if (str[0] == '\"' || str[0] == '\'' && (str[2] == '\"' ||  str[2] == '\'' && str[2] == '\''))
				return(printf("%c: command not found\n", str[1]), 0);
	}
	else if (len == 2)
	{
		if (str[0] == '\"' || str[0] == '\'' && (str[1] == '\"' ||  str[1] == '\'' && str[1] == '\''))
							return(0);
	}
	return (1);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_shell		*element;
	t_token		*node;
	t_token		*head;
	t_cmd		*t_head;
	t_env		*env;
	t_cmd		*hd_temp;
	int			hd_res;
	int			*exit_status;
	t_heredoc	*heredoc;

    head = NULL;
    t_head = NULL;
    
	if (!envp || envp[0] == NULL)
		return (printf("Error, no env detected.\n"), 1);
	env = copy_env(envp);
	if (!env)
		return (printf("Error, copying the env.\n"), 1);
	element = NULL;
	exit_status = (int*)malloc(sizeof(int));
	*exit_status = 0;
	while (1)
	{
		head = NULL;
		t_head = NULL;
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = readline("\033[1;34mminishell>\033[0m ");
		//line = "echo $\'$\'$\'$\'";
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		// if (!if_valid(line))
		// {
		// 	free(line);
		// 	continue;
		// }
        if (line && *line)
                add_history(line);

		if (line != NULL && check_quotes(head, line))
		{
			node = lexer(line, env);
			head = node;
         //   print_list(head);
			if (check_tokens(head))
			{ 
				t_head = init_cmds(node, env);
				if (ft_strcmp(t_head->cmd, "exit") == 0)
				{
					free_tokens(head);
					free_cmds(t_head);
					break ;
				}
				hd_temp = t_head;
				if (hd_temp->heredoc_delim)
				{
					heredoc = init_heredoc_struct(hd_temp);
					free_tokens(head);
                    clear_history();
					do_struct(&element, t_head, exit_status);
                    hd_res = init_heredoc(heredoc, env, line, element);
                    free(exit_status);
					exit_status = malloc(sizeof(int));
					if (!exit_status)
						return 1;
					*exit_status = hd_res;
					free_cmds(t_head);
					head = NULL;
					t_head = NULL;
                    line = NULL;
                    env = copy_env(envp);
				}
				else
				{
					do_struct(&element, t_head, exit_status);
					argc = count_commands(element);
					do_commands(element, &env, argc);
				}
			}
            if (line)
			    rl_free(line);
		}
		if (element != NULL)
		{
			free_shell(element);
			element = NULL;
		}
        if (head)
		    free_tokens(head);
        if (t_head)
		    free_cmds(t_head);
		rl_free_undo_list(); //Fixes some direct leaks from readline but not sure of we can use it
	}
   if (exit_status)
	   free(exit_status);
	if (line)
		rl_free(line);
    if (env)
	      env = free_env_list_tmp(env); // causes seg on ctrl + D **sobs**
   // clear_history();
	return (0);
}
