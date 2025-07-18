/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/18 13:50:45 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdio.h>
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
	t_env	*nd;
	t_env	*prev;

	if (!ft_strncmp(elem->command->cmd, "pwd", 3))
	{
		return(do_pwd());
	}
	else if (!ft_strncmp(elem->command->cmd, "kill", 4))
	{
		exit(0);
	}
	else if (!ft_strncmp(elem->command->cmd, "env", 3))
	{
		return (do_env(env));
	}
	else if (!ft_strncmp(elem->command->cmd, "unset", 5))
	{
		nd = (*env);
		prev = nd;
		while (nd)
		{
			if (!ft_strncmp(elem->command->args[0], nd->key,
					ft_strlen(elem->command->args[0])))
			{
				delete_node(env, nd, prev);
				break ;
			}
			prev = nd;
			nd = nd->next;
		}
	}
	else if (!ft_strncmp(elem->command->cmd, "echo", 4))
	{
		return (do_echo(elem, env));
	}
	else if (!ft_strncmp(elem->command->cmd, "export", 6))
	{
		return (do_export(elem,env));
	}
	else if (!ft_strncmp(elem->command->cmd, "cd", 2))
	{
		return do_cd(elem, env);
	}
	return (0);
}

void close_pipes(int p[2][2], int count, int has_next)
{
    if (count > 0)
    {
        close(p[(count - 1) % 2][0]);
        close(p[(count - 1) % 2][1]);
    }
    if (!has_next && count >= 0)
    {
        close(p[count % 2][0]);
        close(p[count % 2][1]);
    }
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
/*
void do_commands(t_shell *elem, t_env **env, int ac)
{
    int pid[ac];
    int p[2][2] = {{-1, -1}, {-1, -1}};
    int fd;
    int count = 0;
    int old_stdout = dup(STDOUT_FILENO);
    char **penv;
	int *result;

	penv = NULL;
	for (int i = 0; i < ac; i++)
		pid[i] = -3;
    while (elem != NULL && elem->type != NULL)
    {
        if (elem->next != NULL)
        {
            if (pipe(p[count % 2]) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }
        if (!ft_strncmp(elem->type, "command", ft_strlen("command")))
        {
			
            pid[count] = fork();
            if (pid[count] == -1)
            {
				if (penv)
				{
			 		free_penv(penv);
			 		penv = NULL;
				}
                perror("fork");
                exit(1);
            }
        }
        if (!ft_strncmp(elem->type, "built-in", ft_strlen("built-in")) || pid[count] == 0)
        {
            if (count > 0 && pid[count] == 0) // Not first command
            {
                if (dup2(p[(count - 1) % 2][0], STDIN_FILENO) == -1)
                {
                    perror("dup2 stdin");
                    exit(1);
                }
            }

            if (elem->next != NULL) // Not last command
            {
                if (dup2(p[count % 2][1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 stdout");
                    exit(1);
                }
            }

            if (elem->command->infile != NULL)
            {
                fd = open(elem->command->infile, O_RDONLY);
                if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
                {
                    perror(elem->command->infile);
                    exit(1);
                }
                close(fd);
            }
            if (elem->command->outfile != NULL)
            {
                fd = elem->command->append ?
                    open(elem->command->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644) :
                    open(elem->command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror(elem->command->outfile);
                    exit(1);
                }
                close(fd);
            }
            if (!ft_strncmp(elem->type, "command", ft_strlen("command")))
            {
				if (elem->command->outfile == NULL && elem->next == NULL)
					dup2(old_stdout, STDOUT_FILENO);
				if (count > 0)
					close(p[(count-1)%2][1]);
				if (elem->next != NULL)
					close(p[count%2][0]);
				if (p[count%2][1] != -1)
					close(p[count%2][1]);
				penv = create_envp(*env);
                exec_command(elem, env, penv);
            }
            else
            {
				if (elem->command->outfile == NULL && elem->next == NULL)
					dup2(old_stdout, STDOUT_FILENO);
                *(elem->exit_status_code) = do_builtins(elem, env);
				if (count > 0)
					close(p[(count-1)%2][1]);
				if (elem->next == NULL)
					close(p[count%2][0]);
				if (p[count%2][1] != -1)
					close(p[count%2][1]);
            }
        }
        else
        {
            if (count > 0)
                close(p[(count - 1) % 2][0]);
            if (elem->next != NULL)
                close(p[count % 2][1]);
			if (p[count%2][1] != -1)
				close(p[count%2][1]);
        }
        count++;
		result = elem->exit_status_code;
        elem = elem->next;
    }
	int i = 0;
	int status;
    while (i < count)
	{
		if (pid[i] != -3)
		{
			waitpid(pid[i],&status, 0);
			if (WIFEXITED(status))
				*(result) = WEXITSTATUS(status);
			else 
				*(result) = 127;
		}
		i++;
	}
    dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout); //not
	if (penv)
		free_penv(penv);
}
*/
void do_commands(t_shell *elem, t_env **env, int ac)
{
    int *pids = calloc(ac+1, sizeof(int));
    int prev_fd = -1;
    int old_stdout = dup(STDOUT_FILENO);
    char **penv = NULL;
    int count = 0;
    int *last_status_ptr = NULL;

    while (elem && elem->type)
    {
        bool has_next = (elem->next != NULL);
        bool is_cmd = (strncmp(elem->type, "command", 7) == 0);
        bool is_builtin = (strncmp(elem->type, "built-in", 9) == 0);

        // Handle standalone built-in without forking
        if (is_builtin && !has_next)
        {
            int code = do_builtins(elem, env);
            *(elem->exit_status_code) = code;
            last_status_ptr = elem->exit_status_code;
            elem = elem->next;
            continue;
        }

        int next_pipe[2] = {-1, -1};
        if (has_next)
        {
            if (pipe(next_pipe) < 0)
                perror("pipe"), exit(1);
        }

        // Fork for external commands or built-ins in pipeline
        int pid = fork();
        if (pid < 0)
            perror("fork"), exit(1);

        if (pid == 0)
        {
            // Child
            if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);
            if (has_next)
                dup2(next_pipe[1], STDOUT_FILENO);

            // Close fds in child
            if (prev_fd != -1) close(prev_fd);
            if (next_pipe[0] != -1) close(next_pipe[0]);
            if (next_pipe[1] != -1) close(next_pipe[1]);

            // File redirections
            if (elem->command->infile)
            {
                int fd = open(elem->command->infile, O_RDONLY);
                if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
                    perror(elem->command->infile), exit(1);
                close(fd);
            }
            if (elem->command->outfile)
            {
                int flags = elem->command->append ? (O_WRONLY|O_CREAT|O_APPEND)
                                                  : (O_WRONLY|O_CREAT|O_TRUNC);
                int fd = open(elem->command->outfile, flags, 0644);
                if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
                    perror(elem->command->outfile), exit(1);
                close(fd);
            }

            if (is_cmd)
            {
                penv = create_envp(*env);
                exec_command(elem, env, penv);
                perror("exec");
                exit(1);
            }
            else // built-in in pipeline
            {
                int code = do_builtins(elem, env);
                exit(code);
            }
        }
        else
        {
            // Parent
            pids[count++] = pid;
            // Close old read end
            if (prev_fd != -1) close(prev_fd);
            // Close write end of new pipe
            if (has_next) close(next_pipe[1]);
            // Next prev_fd is read end of new pipe
            prev_fd = has_next ? next_pipe[0] : -1;
        }

        last_status_ptr = elem->exit_status_code;
        elem = elem->next;
    }

    // Close any leftover
    if (prev_fd != -1) close(prev_fd);

    // Wait for children
    for (int i = 0; i < count; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            *last_status_ptr = WEXITSTATUS(status);
        else
            *last_status_ptr = 127;
    }

    dup2(old_stdout, STDOUT_FILENO);
    close(old_stdout);
    free(pids);
    if (penv) free_penv(penv);
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

char **create_envp(t_env *env)
{
	char	**penv;
	char	*str;
	char	*str2;
	int		i;
	int		len;
	
	i = 0;
	len = env_len(env);
	penv = malloc(sizeof(char *) * (len + 1));
	if (!penv)
		return (NULL);
	while (i < len && env)
	{
		if (!ft_strncmp(env->key, "SHLVL", 5))
		{
			if (ft_atoi(env->value) >= 1000)
				env->value = ft_itoa(1);
			str2 = env->value;
			env->value = ft_itoa(ft_atoi(env->value) + 1);
			free(str2);
		}
		str = ft_strjoin(env->key, "=");
		if (!str)
			return (NULL);
		str2 = ft_strjoin(str, env->value);
		free(str);
		if (!str2)
			return (NULL);
		penv[i] = str2;
		env = env->next;
		i++;
	}
	penv[i] = NULL;
	return (penv);
}


t_env *copy_env(char *envp[])
{
    t_env *head;
    t_env *tail;
    t_env *new_node;
    int i;
	int shlvl;

	head = NULL;
	i = 0;
	shlvl = 0;
    while (envp[i])
    {
		if (!ft_strncmp(envp[i], "SHLVL=", 5))
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
	{
		new_node = create_node("SHLVL=2");
		tail->next = new_node;
		new_node->next = NULL;
	}
    return (head);
}

/*
t_env   *copy_env(char *envp[])
{

    t_env   *head;
    t_env   *node;
    t_env   *nnode;
    int     i;
	int		shlvl;

    head = create_node(envp[0]);
	shlvl = 0;
    if (!head)
        return (NULL);
    i = 2;
    node = create_node(envp[1]);
    if (!node)
        return (NULL);
    head->next = node;
    while (envp[i])
    {
        nnode = create_node(envp[i]);
        if (!nnode)
            return (NULL);
        node->next = nnode;
        node = node->next;
        i++;
    }
    return (head);
}*/

void	free_split(char **sp)
{
	int	i;

	i = 0;
	while (sp[i])
	{
		free(sp[i]);
		i++;
	}
	free(sp);
}

char	*get_element(char *line)
{
	char	**split;

	split = ft_split(line, ' '); //Free this split
	if (split && (!ft_strncmp(split[0], "pwd", 4) || !ft_strncmp(split[0],
				"kill", 5) || !ft_strncmp(split[0], "env", 4)
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

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_shell		*element;
	t_token		*node;
	t_token		*head;
	t_cmd		*t_head;
	t_env		*env;
	t_cmd		*hd_temp;
	int			heredoc_exit_status;
	int			hd_res;
	int			*exit_status;
	t_heredoc	*heredoc;

	if (!envp)
		return (printf("Error, no env detected.\n"), 1);
	env = copy_env(envp);
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
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line != NULL && check_quotes(head, line))
		{
			node = lexer(line);
			head = node;
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
				while (hd_temp->next)
					hd_temp = hd_temp->next;
				if (hd_temp->heredoc_delim)
				{
					heredoc = init_heredoc_struct(hd_temp);
					free_tokens(head);
					free_cmds(t_head);
					head = NULL;
					t_head = NULL;
					hd_res = init_heredoc(heredoc, env, line);
				}
				else
				{
					do_struct(&element, t_head, exit_status);
					argc = count_commands(element);
					do_commands(element, &env, argc);
				}
			}
			rl_free(line);
		}
		if (element != NULL)
		{
			free_shell(element);
			element = NULL;
		}
		free_tokens(head);
		free_cmds(t_head);
	}
	free(exit_status);
	if (line)
		rl_free(line);
	free_env_list_tmp(env);
	return (0);
}
