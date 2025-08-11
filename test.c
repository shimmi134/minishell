/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/11 14:23:04 by joshapir         ###   ########.fr       */
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
        if (is_builtin && !has_next)
        {
			do_outfile(elem->command->outfile, elem->command->append);
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
        int pid = fork();
        if (pid < 0)
            perror("fork"), exit(1);
        if (pid == 0)
        {
			close_and_open_child(prev_fd, has_next, next_pipe);
			do_infile(elem->command->infile);
            do_outfile(elem->command->outfile, elem->command->append);
            if (elem->command->cmd != NULL && is_cmd) // Have to use execute_command function in exec utils
            {
                penv = create_envp(*env);
                exec_command(elem, env, penv);
            }
            else if (is_builtin)// built-in in pipeline
                exit(do_builtins(elem, env));
        }
        else
        {
            pids[count++] = pid;
			prev_fd = close_parent(prev_fd, has_next, next_pipe);
        }
        last_status_ptr = elem->exit_status_code;
        elem = elem->next;
    }
    if (prev_fd != -1) close(prev_fd);
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
	int			heredoc_exit_status;
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
		//line = "|";
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
            print_list(head);
			if (check_tokens(head))
			{ 
				;
				// t_head = init_cmds(node, env);
				// if (ft_strcmp(t_head->cmd, "exit") == 0)
				// {
				// 	free_tokens(head);
				// 	free_cmds(t_head);
				// 	break ;
				// }
				// hd_temp = t_head;
				// if (hd_temp->heredoc_delim)
				// {
				// 	heredoc = init_heredoc_struct(hd_temp);
				// 	free_tokens(head);
                //     clear_history();
				// 	do_struct(&element, t_head, exit_status);
                //     hd_res = init_heredoc(heredoc, env, line, element);
                //     free(exit_status);
				// 	exit_status = malloc(sizeof(int));
				// 	if (!exit_status)
				// 		return 1;
				// 	*exit_status = hd_res;
				// 	free_cmds(t_head);
				// 	head = NULL;
				// 	t_head = NULL;
                //     line = NULL;
                //     env = copy_env(envp);
				// }
				// else
				// {
				// 	do_struct(&element, t_head, exit_status);
				// 	argc = count_commands(element);
				// 	do_commands(element, &env, argc);
				// }
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
