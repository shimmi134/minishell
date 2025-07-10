/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/07/10 15:49:52 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	**split;
	t_env	*node;
	char	*str;
	t_env	*temp;
	char	*oldpwd;

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
		int check = corr_input(elem);
		if (check)
		{
			split = ft_split(elem->command->args[0], '=');
			node = create_node(elem->command->args[0]);
			if (split)
			{
				str = split[1];
				for (int i = 2;  i < ft_lensplit(split); i++)
				{
					if (split[i])
						str = ft_strjoin(str, "=");
					str = ft_strjoin(str, split[i]); // leaks
				}
				if (split[0][(int)ft_strlen(split[0]) - 1] != '+')
				{
					nd = (*env);
					while (nd)
					{
						if (!ft_strncmp(split[0], nd->key, ft_strlen(split[0])))
						{
							nd->value = str;
							break ;
						}
						nd = nd->next;
					}
					if (!nd)
					{
						node->value = str;
						addlast(env, node);
					}
				}
				else
				{
					nd = (*env);
					split[0] = ft_strtrim(split[0], "+");
					while (nd != NULL)
					{
						if (!ft_strncmp(nd->key, split[0], ft_strlen(split[0])))
						{
							nd->value = ft_strjoin(nd->value, str);
							free(str);
							break ;
						}
						nd = nd->next;
					}
					if (nd == NULL)
					{
						node->key = split[0];
						node->value = str;
						addlast(env, node);
					}
				}
			}
			else
			{
				t_env *tmp = sort_list(env);
				while (tmp)
				{
					printf("declare -x %s", tmp->key);
					if (tmp->value)
						printf("=\"%s\"", tmp->value);
					printf("\n");
					tmp = tmp->next;
				}
			}
		}
		else {
			int i = 1;
			while (i < ft_lensplit(elem->command->args))
			{
				printf("export: '%s': not a valid identifier\n", elem->command->args[i]);
				i++;
			}
			return 1;
		}
	}
	else if (!ft_strncmp(elem->command->cmd, "cd", 2))
	{
		oldpwd = getcwd(NULL, 0);
		i = chdir(elem->command->args[0]);
		if (i != -1)
		{
			str = getcwd(NULL, 0);
			temp = *env;
			while (temp)
			{
				if (!ft_strncmp(temp->key, "PWD", 3))
					temp->value = str;
				else if (!ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)))
					temp->value = oldpwd;
				temp = temp->next;
			}
		}
		else if (!ft_strncmp("-\0", elem->command->args[0], 2))
		{
			str = getcwd(NULL,0);
			temp = *env;
			while (temp && ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)) != 0)
				temp = temp->next;
			oldpwd = temp->value;
			i = chdir(oldpwd);
			if (i != -1)
			{
				temp = *env;
				while (temp)
				{
					if (!ft_strncmp(temp->key, "PWD", 3))
						temp->value = oldpwd;
					else if (!ft_strncmp(temp->key, "OLDPWD", ft_strlen(temp->key)))
						temp->value = str;
					temp = temp->next;
				}
			}
		}
		if (i == -1)
		{
			printf("cd: %s: %s\n", strerror(errno), elem->command->args[0]);
			return (1);
		}
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

void do_commands(t_shell *elem, t_env **env, int ac)
{
    int id = 0;
    int p[2][2] = {{-1, -1}, {-1, -1}};
    int fd;
    int count = 0;
    int old_stdout = dup(STDOUT_FILENO);
    char **penv = create_envp(*env);

    while (elem != NULL && elem->type != NULL)
    {
        // Create pipe if not last command
        if (elem->next != NULL)
        {
            if (pipe(p[count % 2]) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }

        // Only fork for commands, not for built-ins
        if (!ft_strncmp(elem->type, "command", ft_strlen("command")))
        {
            id = fork();
            if (id == -1)
            {
                perror("fork");
                exit(1);
            }
        }

        // CHILD PROCESS or BUILT-IN
        if (!ft_strncmp(elem->type, "built-in", ft_strlen("built-in")) || id == 0)
        {
            // Set up input redirection
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
				close(p[count%2][1]);
                exec_command(elem, env, penv);
                perror("exec_command"); // Only reached if exec fails
                exit(1);
            }
            else
            {
                do_builtins(elem, env);
                dup2(old_stdout, STDOUT_FILENO);
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
    close(old_stdout);
    close_pipes(p, count, 0);
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
	penv = malloc(sizeof(char *) * (len+1));
	if (!penv)
		return (NULL);
	while (i < len)
	{
		str = ft_strjoin(env->key, "=");
		if (!str)
			return (NULL);
		str2 = ft_strjoin(str, env->value);
		if (!str2)
			return (NULL);
		free(str);
		penv[i] = malloc(sizeof(ft_strlen(str2))+1);
		penv[i] = str2;
		penv[i][ft_strlen(str2)] = '\0';
		env = env->next;
		i++;
	}
	penv[i] = NULL;
	return (penv);
}

t_env	*copy_env(char *envp[])
{
	t_env	*head;
	t_env	*node;
	t_env	*nnode;
	int		i;

	head = create_node(envp[0]);
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
		return ("built-in");
	}
	else
	{
		return ("command");
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
					heredoc_exit_status = init_heredoc(hd_temp, env, line);
				else
				{
					do_struct(&element, t_head);
					argc = count_commands(element);
					do_commands(element, &env, argc);
				}
			}
		}
		free_tokens(head);
		free_cmds(t_head);
	}
	free_env_list_tmp(env);
	return (0);
}
