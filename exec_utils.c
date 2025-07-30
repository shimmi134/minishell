#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void do_infile(char *infile)
{
    int fd;

    if (infile != NULL)
    {
        fd = open(infile, O_RDONLY);
        if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
            perror(infile), exit(1);
        close(fd);    
    }
}

void do_outfile(char *outfile, int append)
{
    int flags;
    int fd;
   
    if (outfile != NULL)
    {
        if (append)
            flags = (O_WRONLY|O_CREAT|O_APPEND);
        else
            flags = (O_WRONLY|O_CREAT|O_TRUNC);
        fd = open(outfile, flags, 0644);
        if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
            perror(outfile), exit(1);
        close(fd);    
    }
}

void close_and_open_child(int prev_fd, int has_next, int* next_pipe)
{
    if (prev_fd != -1)
        dup2(prev_fd, STDIN_FILENO);
    if (has_next)
        dup2(next_pipe[1], STDOUT_FILENO);

}
/*
void execute_command(t_shell *elem, t_env **env)
{
    char    **penv;

    if (elem->command->cmd != NULL && ft_strncmp(elem->type, "command", 7) == 0)
    {
        penv = create_envp(*env);
        exec_command(elem, env, penv);
    }
    else if (ft_strncmp(elem->type, "built-in", 9))// built-in in pipeline
        exit(do_builtins(elem, env));
}*/

int close_parent(int prev_fd, int has_next, int* next_pipe)
{
    if (prev_fd != -1) 
        close(prev_fd);
    if (has_next)  
        close(next_pipe[1]);
    if (has_next)
        return next_pipe[0];
    else
        return -1;
}