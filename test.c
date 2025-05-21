/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/05/21 12:18:23 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <sys/wait.h>
#include "minishell.h"
#include <readline/readline.h>
#include <errno.h>

void	delete_node(t_env **env, t_env *target, t_env *prev)
{
	if (prev == target)
		*env = target->next;
	else
		prev->next = target->next;
	free(target);
}

int	count_len(char **sp)
{
	int	i;

	i = 0;
	while (sp && sp[i])
		i++;
	return (i);
}

void	addlast(t_env **env, t_env *add)
{
	t_env	*temp;

	temp = *env;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = add;
}

t_env	*create_node(char *env)
{
	char	**split;
	t_env	*node;

	split = ft_split(env, '=');
	if (!split)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split[0];
	node->value = split[1];
	return (node);
}

int ft_strspn(char *str, char *sep)
{
	int	i;
	int	j;

	if (!str || !sep)
		return (0);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (sep[j])
		{
			if (sep[j] == str[i])
				break;
			j++;
		}
		if (!sep[j])
			return (i);
		i++;
	}
	return (i);
}

void	do_builtins(t_shell *elem, t_env **env)
{
	char	*buf;
	t_env	*nd;
	int		i;
	t_env	*prev;
	int		newline;
	char	**split;
	t_env	*node;
	t_env	*tmp;
	char	*str;
	t_env	*temp;

	i = 0;
	if (!ft_strncmp(elem->command->cmd, "pwd", 3))
	{
		buf = getcwd(NULL, 0);
		if (!buf)
			perror("getcwd");
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
			if (!ft_strncmp(elem->command->args[1], nd->key, ft_strlen(elem->command->args[1])))
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
//		printf("[%s]\n", elem->command->args[0]);
//		printf("[%s]\n", elem->command->args[1]);
		newline = 1;
		i = 0;
		while (i < count_len(elem->command->args) && !ft_strncmp(elem->command->args[i], "-n", 2) && ft_strlen(elem->command->args[i]) == ft_strspn(elem->command->args[i], "-n"))
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
		split = ft_split(elem->command->args[0], '=');
		node = create_node(elem->command->args[0]);
		if (split)
		{
			if (split[0][(int)ft_strlen(split[0]) - 1] != '+')
			{
				nd = (*env);
				while (nd)
				{
					if (!ft_strncmp(split[0],nd->key, ft_strlen(split[0])))
					{
						nd->value = node->value;
						break;
					}
					nd = nd->next;
				}
				if (!nd)
					addlast(env, node);
			}
			else
			{
				nd = (*env);
				split[0] = ft_strtrim(split[0], "+");
				while (nd != NULL)
				{
					if (!ft_strncmp(nd->key, split[0], ft_strlen(split[0])))
					{
						nd->value = ft_strjoin(nd->value, split[1]);
						break ;
					}
					nd = nd->next;
				}
				if (nd == NULL)
				{
					node->key = split[0];
					node->value = split[1];
					addlast(env, node);
				}
			}
		}
		else
		{
			tmp = *env;
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
	else if (!ft_strncmp(elem->command->cmd, "cd", 2))
	{
		char *oldpwd = getcwd(NULL, 0);
		i = chdir(elem->command->args[1]);
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
		else{
			printf("%s\n",strerror(errno));
		}
		// return (i);
	}
}

char	*get_paths(t_env **env)
{
	t_env *nd;

	nd = *env;
	while (nd)
	{
		if (ft_strncmp(nd->key, "PATH", 4) == 0)
			return (nd->value);
		nd = nd->next;
	}
	return (NULL);
}

char	*try_paths(char	**split, char *comm)
{
	char	*temp;
	char	*temp2;
	int		i;
	
	i = 0;
	while (split[i])
	{
		temp = ft_strjoin("/", comm);
		temp2 = ft_strjoin(split[i], temp);
		if (access(temp2, F_OK) == 0)
			return free(temp), temp2;
		free(temp2);
		free(temp);
		i++;
	}
	return (NULL);
}

char **join_args(char *cmd, char**args)
{
	int	len;
	int	i;
	char	**final_args;

	i = 1;
	len = count_len(args) + 1;
	final_args = malloc((len+1) * sizeof(char *));
	if (!final_args)
		return NULL;
	final_args[0] = cmd;
	final_args[len] = NULL;
	while (i < len)
	{
		final_args[i] =  args[i-1];
		i++;
	}
	return final_args;
}

void exec_command(t_shell *elem, t_env **env, char **envp)
{
	char	*path;
	char	*paths;
	char	**args;
	char	**split;

	if (access(elem->command->cmd, F_OK) != 0)
	{
		paths = get_paths(env);
		if (!paths)
			exit(1);
		split = ft_split(paths, ':');
		path = try_paths(split, elem->command->cmd);
		if (!path)
		{
			perror(elem->command->cmd);
			exit(errno);
		}
	}
	else
		path = elem->command->cmd;
	args = join_args(elem->command->cmd, elem->command->args);
	execve(path, args, envp);
	printf("Error: %s\n", strerror(errno));
	exit(127);
}

void	do_commands(t_shell *elem, t_env **env, char **envp)
{
	int id;

	id = fork();
	if (id == -1)
	{
		perror("fork: ");
		exit(1);
	}
	if (id == 0) // child
		exec_command(elem, env, envp);
	else
		wait(NULL);
}

void	do_element(t_shell *elem, t_env **env, char**envp)
{
	if (!ft_strncmp(elem->type, "built-in", ft_strlen(elem->type)))
		do_builtins(elem, env);
	else if (!ft_strncmp(elem->type, "command", ft_strlen(elem->type)))
		do_commands(elem, env, envp);
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
	char **split = ft_split(line, ' ');
	if (split && (!ft_strncmp(split[0], "pwd", 4) || !ft_strncmp(split[0], "kill", 5)
		|| !ft_strncmp(split[0], "env", 4) || !ft_strncmp(split[0], "unset",
			6) || !ft_strncmp(split[0], "echo", 5) || !ft_strncmp(split[0],
			"export", 6) || !ft_strncmp(split[0], "cd", 2)))
	{
		return ("built-in");
	}
	else{
		return ("command");
	}
}

void	do_struct(t_shell **element, t_cmd *command)
{
	int	i;
	t_shell *temp;

	while(command)
	{
		if (!(*element)->command)
		{
			(*element) = malloc(sizeof(t_shell));
			if (!(*element))
				exit(1);
		}
		else if (!(*element)->next)
		{
			(*element)->next = malloc(sizeof(t_shell));
			if (!(*element)->next)
				exit(1);
			(*element) = (*element)->next;
		}
		(*element)->command = malloc(sizeof(t_cmd));
		if (!(*element)->command)
			exit(2);
		(*element)->command = command;
		(*element)->type = get_element(command->cmd);
		command = command->next;
	}
}


// TODO:
// Make it so I transform all of line into nodes of t_shell
//
int	main(int ac, char *argv[], char *envp[])
{
	t_shell	*element;
	t_cmd	*command;
	t_token *node;
	t_token *head;
	t_env	*env;
	char	*line;
	char	**split;

	env = copy_env(envp);
	split = argv;
	while (1)
	{
		line = readline(">>> ");
		/*if (!line)
			return 1;*/
		if (line != NULL && ft_strncmp(line, "", 1))
		{
			node = lexer(line);
			head = node;
			if (check_tokens(head))
			{
				command = init_cmds(node,env);
				do_struct(&element,command);	
				/*
				element = malloc(sizeof(t_shell));
				element->command = malloc (sizeof(t_cmd));
				element->command = command;
				element->type = get_element(command->cmd);
				element->command->args = command->args;
				*/
				ac = count_len(command->args);
				do_element(element, &env, envp);
			}
		}
	}
}
