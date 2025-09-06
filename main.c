/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:36:52 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/06 01:20:27 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_code = 0;

int	check_out_in(t_shell *elem)
{
	int	fd;
	int	flags;

	flags = 0;
	if (elem->command->infile)
	{
		fd = open(elem->command->infile, O_RDONLY);
		if (fd < 0 && (*elem->exit_status_code) == 0)
		{
			(*elem->exit_status_code) = 1;
			perror(elem->command->infile);
			return (1);
		}
		close(fd);
	}
	if (elem->command->outfile)
	{
		set_flags(&flags, elem->command->append);
		fd = open(elem->command->outfile, flags, 0644);
		if (fd < 0 && (*elem->exit_status_code) == 0)
		{
			(*elem->exit_status_code) = 1;
			perror(elem->command->outfile);
			return (1);
		}
		close(fd);
	}
	return (0);
}

pid_t	command_fork(t_shell *elem, t_env **env, int *prev_fd)
{
	int		next_pipe[2];
	pid_t	pid;
	int		need_next;
	int		next_read;
	int		next_write;
	int		flag;

	flag = 0;
	init_rw(elem, &need_next, &next_read, &next_write);
	next_pipe[0] = -1;
	next_pipe[1] = -1;
	prepare_pipe(next_pipe, need_next, &next_read, &next_write);
	if (check_out_in(elem))
		flag = 1;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (pid < 0)
		exit(1);
	if (pid == 0)
	{
		if (next_read != -1)
			close(next_read);
		if (!flag)
			child_process(elem, env, *prev_fd, next_write);
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	close_prev_next(prev_fd, next_read, next_write);
	return (pid);
}

void	execute_loop(t_shell *elem, t_env **env, int *fd_val,
		int **last_status_ptr_out)
{
	int	*pids;
	int	pid;
	int	count;

	count = 0;
	pids = calloc((count_commands(elem)) + 1, sizeof(int));
	while (elem && elem->type)
	{
		if (elem->command->heredoc)
		{
			heredoc_execute_loop(elem, env, fd_val);
			break ;
		}
		if (execute_loop_loop(elem, env, last_status_ptr_out, fd_val) == 0)
		{
			pid = command_fork(elem, env, fd_val);
			pids[count++] = (int)pid;
			*last_status_ptr_out = elem->exit_status_code;
			elem = elem->next;
		}
		else
			break ;
	}
	wait_children(pids, count, *last_status_ptr_out, fd_val);
}

void	do_commands(t_shell *elem, t_env **env, int fd_val)
{
	int	old_stdout;
	int	old_stdin;
	int	*last_status_ptr;

	old_stdout = -1;
	old_stdin = -1;
	old_stdout = dup(STDOUT_FILENO);
	old_stdin = dup(STDIN_FILENO);
	execute_loop(elem, env, &fd_val, &last_status_ptr);
	if (old_stdout != -1)
		dup2(old_stdout, STDOUT_FILENO);
	if (old_stdout != -1)
		close(old_stdout);
	if (old_stdin != -1)
		dup2(old_stdin, STDIN_FILENO);
	if (old_stdin != -1)
		close(old_stdin);
}

void print_cmd_list(t_cmd *head) 
{
    int i;

    i = 0;
    t_cmd *current = head;
    while (current != NULL) 
	{
        printf("\n-----------------------\n");
        if (current->cmd)
        {
            if (current->cmd[0] == '\0')
                printf("cmd = [empty]\n");
            else
                printf("cmd = %s\n", current->cmd);
        }
		 if (current->append)
		 	printf("[append]");
		 if (current->redirect)
		 	printf("[redirect]");
         if (current->heredoc)
		 	printf("[heredoc] ");
       // if (current->heredoc_delim)
       //     printf("heredoc_delim = %s\n", current->heredoc_delim);
		if (current->infile)
		 	printf("infile = %s\n", current->infile);
        if (current->outfile)
			printf("outfile = %s\n", current->outfile);
		if (current->exit_status)
			printf("return exit status\n");
        if (current->args[i])
           printf("args = ");
       while(current->args[i])
       {
            if (current->args[i][0] == '\0')
                printf("[empty]\n");
            else
                printf("%s ", current->args[i]);
            i++;
       }
        current = current->next;
        i = 0;
    }
    printf("\n-----------------------\n");
    //printf("NULL\n");
}

int	init_execute(t_token *node, t_token *head, t_env **env, int *exit_status)
{
	t_cmd	*t_head;
	t_shell	*element;

	element = NULL;
	t_head = init_cmds(node, *exit_status, *env);
	//print_cmd_list(t_head);
	if (pre_struct_exit(t_head, exit_status, *env, head))
		return (1);
	do_struct(&element, t_head, exit_status);
	do_commands(element, env, -1);
	if (t_head)
		free_cmds(t_head);
	if (element != NULL)
	{
		free_shell(element);
		element = NULL;
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	t_env	*env;
	int		*exit_status;

	if (!pre_start_check(argc, argv, envp))
		return (1);
	exit_status = (int *)malloc(sizeof(int));
	*exit_status = 0;
	env = copy_env(envp);
	while (1)
	{
		if (!env)
			return (printf("Error, copying the env.\n"), 1);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = readline("\033[1;34mminishell>\033[0m ");
		check_code(exit_status);
		if (check_line(line) == 1)
			break ;
		if (pre_exec(line, &env, exit_status) == 1)
			break ;
	}
	clear_history();
	free_combined(exit_status, env);
	return (0);
}
