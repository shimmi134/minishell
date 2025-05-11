#include "minishell.h"

int check_tokens (t_token *tokens)
{
    t_token *head;
    int prev_type;

    head = tokens;
if (!tokens ||tokens->type == TOKEN_PIPE || tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT)
        {
            printf("Syntax error, invalid token at start\n");
            return (0) ;
        }       
    while (tokens)
    {
        //printf("token = %d\n", tokens->type);
        prev_type = tokens->type;
        if (tokens->type == TOKEN_PIPE)
        {
           // printf("goes in\n");
            prev_type = tokens->type;
            if (!tokens->next)
            {
                printf("Syntax error: pipe without command\n");
                return (0);
            }
            //tokens = tokens->next;
            if (tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error: pipe without command\n");
                return (0);
            }
             //printf("token type = %d\n", tokens->type);
        }
         else if(tokens->type == TOKEN_HEREDOC)
         {
            if (!tokens->next || tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error near unexpected token\n");
                break ;
            }
         }
        else if (tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT)
        {
            if (!tokens->next)
            {
                printf("Syntax error: expected filename\n");
                return (0);
            }
            //printf("goes in\n");
            else if ((tokens->next->type) && tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error: expected filename\n");
               // exit(EXIT_FAILURE);
               return (0);
            }
        }
         tokens = tokens->next;
    }
    return (1);
}

int arg_count(t_token *tokens)
{
    int count;

    count = 0;
    while (tokens && tokens-> type == TOKEN_WORD)
    {
            count++;
            tokens = tokens->next;
    }
    return(count);
}

t_cmd *new_cmd_token(t_token *tokens)
{
    int count;
    
    t_cmd *cmd = malloc(sizeof(t_cmd));
    count = arg_count(tokens) - 1;
        cmd->args = malloc(sizeof(char *) * (count + 1));
    if (!cmd->args)
        return(NULL);
    if (tokens->type == TOKEN_WORD)
        cmd->cmd = tokens->value;
    else
        cmd->cmd = tokens->next->value;
    cmd->args[0] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->heredoc = 0;
    //cmd->args[0] = NULL;
    cmd->next = NULL;
    return (cmd);
}

t_token *assign_args(t_token *tokens, t_cmd *cmds)
{
    t_token *current_t;
    t_cmd *current_c;
    int i;

    i = 0;
    //current_t = *(tokens);
    //current_c = *(cmds);
    while (tokens && tokens->type == TOKEN_WORD)
    {
        printf("token->value = %s\n", tokens->value);
        cmds->args[i] = tokens->value;
        i++;
        tokens = tokens->next;
    }
    cmds->args[i] = NULL;
  //  printf("token type at end of assign = %d\n", tokens->type);
    return (tokens);
}
t_token *assign_ctl_tokens(t_token *token, t_cmd *cmd)
{
    int type;

    type = token->type;

    if (type == TOKEN_REDIRECT_IN)
    {
         if (token->next)
            token = token->next;
        cmd->infile = ft_strdup(token->value);
        token = token->next;
    }
    else if (type == TOKEN_REDIRECT_OUT)
    {  
        if (token->next)
            token = token->next;
        cmd->outfile = ft_strdup(token->value);
        token = token->next;
    }
    else if (type == TOKEN_HEREDOC)
    {
        cmd->heredoc = 1;
        if (token->next)
            cmd->heredoc_delim = ft_strdup(token->next->value);
        else    
            printf("Error: no heredoc delim found\n");
        exit (EXIT_FAILURE);
    }
    else if (type == TOKEN_APPEND)
    {
        cmd->append = 1;
        cmd->outfile = ft_strdup(token->value);
    }
    return (token);
}
void init_cmds(t_token *tokens)
{
    t_cmd *head;
    t_cmd *cmds;
    int i;
    int type;

    i = 0;
  //  prev_type = 10;
    cmds = new_cmd_token(tokens);
    head = cmds;
 //   cmds->cmd = tokens-> value;
    //if (tokens->next)
        tokens = tokens->next;
    while (tokens)
    { 
        type = tokens->type;
   //     prev_type = tokens->type;
   //     if (prev_type != TOKEN_WORD)
     //       tokens=tokens->next;
        if (type == TOKEN_WORD)
            tokens = assign_args(tokens, cmds);
        else if (type == TOKEN_PIPE)
        {
            cmds->next = new_cmd_token(tokens);
            tokens = tokens->next->next;
            cmds = cmds->next;
        }
        else
            tokens = assign_ctl_tokens(tokens, cmds);
    }
    print_cmd_list(head);
}

void print_cmd_list(t_cmd *head) 
{
    int i;

    i = 0;
    t_cmd *current = head;
    while (current != NULL) 
	{
        printf("\n-----------------------\n");
		//print_enum(current);
        printf("cmd = %s\n", current->cmd);
		if (current->append)
			printf("[append]");
        if (current->heredoc)
			printf("[heredoc]");
		if (current->infile)
			printf("infile = %s\n", current->infile);
        if (current->outfile)
			printf("outfile = %s\n", current->outfile);
        if (current->args[i])
            printf("args = ");
        while(current->args[i])
            printf("%s ", current->args[i++]);
        current = current->next;
        i = 0;
    }
    printf("\n-----------------------\n");
    //printf("NULL\n");
}