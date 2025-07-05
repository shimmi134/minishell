# include "../minishell.h"

void handle_sigint_heredoc(int sig_int)
{
	(void)sig_int;
	exit(1);
}

char *heredoc_expand(char *str, t_env * env)
{
	int i;
	int j;
	char *tmp;
	char *tmp2;
	char *expanded;

	i = 0;
	j = 0;
	tmp = NULL;
    expanded = NULL;
	while(str[i] != '$')
		i++;
	if (i > 0)
	{
		tmp = malloc((sizeof(char *) * i) + 1);
		i = 0;
		while (str[i] != '$')
		{
			tmp[i] = str[i];
			i++;
		}
		tmp[i] = '\0';
	}
	i++;
	if (str[i])
	{
		j = 0;
		while (str[i])
		{
			i++;
			j++;
		}
		//i = ft_strlen(str) - j;
		tmp2 = malloc((sizeof(char *) * j) + 1);
		//   i = ft_strlen(str) - j;
		//i = j;
		j = 0;
		while (str[j] != '$')
			j++;
		j++;
		i = 0;
		while (str[j])
		{
			tmp2[i] = str[j];
			i++;
			j++;
		}
		tmp2[i] = '\0';
		expanded = expand_var(tmp2, env);
		if (tmp2)
			free (tmp2);
		if (expanded)
        {
            if (tmp)
			{
			    tmp2 = ft_strjoin(tmp, expanded);
				free(tmp);
			}
            else
                tmp2 = expanded;
            return(tmp2);
        }
		else
			return (tmp);
	}
	return (NULL);
}

int init_heredoc(t_cmd *hd_temp, t_env *env, char *line)
{
	pid_t pid;
			if (hd_temp->heredoc_delim)
		{
			hd_temp->heredoc_fd = read_heredoc(hd_temp->heredoc_delim, hd_temp->heredoc_quoted, env);
			if (hd_temp->heredoc_fd == -1)
    		{
        		  free(line);
            		return(0) ;
    		}
		
				pid = fork();
				if (pid == 0)
				{
					if (hd_temp->heredoc_fd != -1)
				{
    				dup2(hd_temp->heredoc_fd, STDIN_FILENO);
    				close(hd_temp->heredoc_fd);
				}
				if (hd_temp->cmd)
					execvp(hd_temp->cmd, hd_temp->args);
			
					// perror("execvp");
				exit(EXIT_FAILURE);
				}
				else if (pid > 0)
				{
					if (hd_temp->heredoc_fd != -1)
						close(hd_temp->heredoc_fd);
						    int status;
					signal(SIGINT, SIG_IGN);
   					 waitpid(pid, &status, 0);
					 free(line);
				}
				else
				{
    				perror("fork");
    				free(line);
				}
				
		}
		return (0);
}
int read_heredoc(char *delimiter, int quoted, t_env *env)
{
	int status;
	char *line;
	char *tmp;
	pid_t pid;
	int delimiter_found;
	int pipefd[2];
	t_token *token;
	t_cmd *cmd;

	delimiter_found = 0;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);

	if (pid == 0)
	{   
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);

		while (1)
		{
			line = readline("> ");

			if (!line)
			{
                // printf("line is null\n");
				//  printf("warning: here-document delimited by end-of-file (wanted `EOF`)\n");
				break ;

			}
			if (ft_strcmp(line, delimiter) == 0)
			{
				delimiter_found = 1;
				free(line);
				break;
			}
			if (strchr(line, '$') && !quoted)
			{
				tmp = heredoc_expand(line, env);
                if (tmp)
				    write (pipefd[1], tmp, strlen(tmp));
                write (pipefd[1], "\n", 1);
				free(tmp);
			}
			else
			{
				// token = lexer(line);

				// cmd = init_cmds(token, env);
				// char quote;
				//quote = NULL;
				// if (line[0] =='"')
				// 	quote = '"';
				// else
				// 	quote = '\'';

				// if (line[0] == '"' || line[0] == '\'')
				// {
				// 	write(pipefd[1], &quote, 1);
				// 	write (pipefd[1], cmd->cmd, strlen(cmd->cmd));
				// 	write(pipefd[1], &quote, 1);
				// }
                
				// else
					write (pipefd[1], line, strlen(line));
				write (pipefd[1], "\n", 1);
				free (line);
			}
		}
		if (!delimiter_found)
		{
			printf("bash: warning: here-document delimited by end-of-file (wanted `%s`)\n", delimiter);
		}
		close(pipefd[1]);
		exit (0);
	}
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == SIGINT)
		{
			printf("> ^C\n");
			return (-1);
		}
	}
	return (pipefd[0]);
}
