/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/12 22:20:32 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdio.h>
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
	char	*buf;
	t_env	*nd;
	int		i;
	t_env	*prev;
	int		newline;
	char	*str;

	i = 0;
	if (!ft_strncmp(elem->command->cmd, "pwd", 3))
	{
		buf = getcwd(NULL, 0);
		if (!buf)
			return (perror("getcwd"),1);
		else
			printf("%s\n", buf);
		free(buf);
	}
	else if (!ft_strncmp(elem->command->cmd, "kill", 4))
	{
		// kill(t_pid val_of_child, signal) MOst likely need to do in loop or smth
		exit(0);
	}
	else if (!ft_strncmp(elem->command->cmd, "env", 3))
	{
		nd = (*env);
		while (nd)
		{
			if (nd->value != NULL)
				printf("%s=%s\n", nd->key, nd->value);
			nd = nd->next;
		}
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
		newline = 1;
		i = 0;
		while (i < count_len(elem->command->args)
			&& !ft_strncmp(elem->command->args[i], "-n", 2)
			&& ft_strlen(elem->command->args[i]) == ft_strspn(elem->command->args[i],
				"-n"))
		{
			newline = 0;
			i++;
		}
		while (i < count_len(elem->command->args))
		{
			printf("%s", elem->command->args[i]);
			if (i < count_len(elem->command->args) - 1)
				printf(" ");
			i++;
		}
		if (newline)
			printf("\n");
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

void do_commands(t_shell *elem, t_env **env, int ac)
{
    int id = 0;
    int p[2][2] = {{-1, -1}, {-1, -1}};
    int fd;
    int count = 0;
    int old_stdout = dup(STDOUT_FILENO);
    char **penv;

	penv = NULL;
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
			
            id = fork();
            if (id == -1)
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
        if (!ft_strncmp(elem->type, "built-in", ft_strlen("built-in")) || id == 0)
        {
            if (count > 0) // Not first command
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
                do_builtins(elem, env);
                dup2(old_stdout, STDOUT_FILENO);
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
        }
        count++;
        elem = elem->next;
    }
    while (count--)
        wait(NULL);
    dup2(old_stdout, STDOUT_FILENO);
	   close(old_stdout); //not
	   close_pipes(p, count, 0); //not
	if (penv)
		free_penv(penv);
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

// char **create_envp(t_env *env)
// {
// 	char	**penv;
// 	char	*str;
// 	char	*str2;
// 	int		i;
// 	int		len;
	
// 	i = 0;
// 	len = env_len(env);
// 	penv = malloc(sizeof(char *) * (len+1));
// 	if (!penv)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		str = ft_strjoin(env->key, "=");
// 		if (!str)
// 			return (NULL);
// 		str2 = ft_strjoin(str, env->value);
// 		if (!str2)
// 			return (NULL);
// 		free(str);
// 		if (!str2)
// 			return(NULL);
// 	//	penv[i] = malloc(sizeof(ft_strlen(str2))+1);
// 		//penv[i] = str2;
// 	//	penv[i][ft_strlen(str2)] = '\0';
// 		env = env->next;
// 		i++;
// 	}
// 	penv[i] = NULL;
// 	return (penv);
// }

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
			printf("%s",env->value);
			str2 = env->value;
			env->value = ft_itoa(ft_atoi(env->value) + 1);
			free(str2);
			printf("%s",env->value);
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

void do_struct(t_shell **element, t_cmd *command)
{
    t_shell *new_node;
    t_shell *last = NULL;

    *element = NULL;
    while (command)
    {
        new_node = malloc(sizeof(t_shell));
        if (!new_node)
            exit(1);
        new_node->command = command;
        new_node->type = get_element(command->cmd);
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
	char	*line;
	t_shell	*element;
	t_token	*node;
	t_token	*head;
	t_cmd	*t_head;
	t_env	*env;
	t_cmd	*hd_temp;
	int heredoc_exit_status;
	int hd_res;
	t_heredoc *heredoc;

	env = copy_env(envp);
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
			t_env *temp = env;
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
					do_struct(&element, t_head);
					argc = count_commands(element);
					do_commands(element, &env, argc);
				}
			}
		}
		free_shell(element);
		free_tokens(head);
		free_cmds(t_head);
		rl_free(line);
	}
	if (line)
		rl_free(line);
	free_env_list_tmp(env);
	rl_clear_history();
	return (0);
}
