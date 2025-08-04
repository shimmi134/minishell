/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:13 by joshapir          #+#    #+#             */
/*   Updated: 2025/08/04 04:11:20 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    check_quotes(t_token *token, char *str)
{
    int i;

    i = 0;

    while (str[i])
    {
        if (str[i] == '"')
        {
                i++;
            while (str[i] && str[i] != '"')
                   i++;
            if (str[i] != '"' || i == 0)
                  return(printf("Error: no closing quotes\n"), 0);
        }
        else if (str[i] == '\'')
        {
                i++;
            while (str[i] && str[i] != '\'')
                    i++;
            if (str[i] != '\'' || i == 0)
                    return(printf("Error: no closing quotes\n"), 0);
        }
        i++;
    }
    return (1);
}
int token_loop (t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next || tokens->next->value[0] == '\0')
                return (printf("Syntax error: pipe without command\n"), 0);
            if (tokens->next->type != TOKEN_WORD && tokens->next->type != TOKEN_VARIABLE)
                    return (printf("Syntax error: pipe without command\n"), 0);
        }
         else if(tokens->type == TOKEN_HEREDOC || tokens->type == TOKEN_APPEND)
         {
            if (!tokens->next || tokens->next->type != TOKEN_WORD)
                    return (printf("Syntax error near unexpected token\n"), 0);
         }
        else if (tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT)
        {
            if (!tokens->next)
                    return (printf("Syntax error: expected filename\n"), 0);
            else if ((tokens->next->type) && tokens->next->type != TOKEN_WORD)
                    return (printf("Syntax error: expected filename\n"), 0);
            if (tokens->next->next)
                return ((printf("Syntax error: unexpected token `%s'\n", tokens->next->next->value), 0));
        }
        tokens = tokens->next;
}
    return (1);
}

int check_tokens (t_token *tokens)
{
    t_token *head;
    int prev_type;

    head = tokens;
    if (!tokens)
        return (0);
if (tokens->type == TOKEN_PIPE)
           return(printf("Syntax error, invalid token at start\n"),0);
        if (tokens->type == TOKEN_VARIABLE && !tokens->next)
                return (printf("Syntax error, invalid token at start\n"), 0);    
    if (!token_loop(tokens))
        return (0);
    return (1);
}

int arg_count(t_token *tokens)
{
    int count = 0;

    if (tokens)
        count = 1;
    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->next && tokens->type == TOKEN_WORD && tokens->next->new_word == 1)
                count++;
        tokens = tokens->next;
    }
    return count;
}


t_cmd *new_cmd_token(t_token *tokens, t_env *envp)
{
    int count;
    int i;
    
    i = 0;
    t_cmd *cmd = malloc(sizeof(t_cmd));
    count = arg_count(tokens);
   // printf("count = %d\n", count);
        cmd->args = calloc((count + 2), sizeof(char *));
    if (!cmd->args)
        return(NULL);
	cmd->args[count] = NULL;    
	cmd->cmd = NULL;
    cmd->args[0] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->heredoc_delim = NULL;
    cmd->heredoc_quoted = 0;
    cmd->append = 0;
    cmd->heredoc = 0;
    cmd->heredoc_fd = -1;
	cmd->exit_status = 0;
    cmd->next = NULL;
    return (cmd);
}

// char *expand_with_quotes(char *str, t_env *env)
// {
//     char *tmp;
//     char *t_val;
//     char *new;
//     int i;
//     char quote;
//     int count;
    
//     count = 0;
//     t_val = NULL;
//     tmp = NULL;
//     quote = str[0];
//     i = 0;
//     i++;
//     while (str[i] != quote)
//         i++;
//     tmp = malloc(sizeof(char) * (i + 1));
//     i = 2;
//     while(str[i] != quote)
//         tmp[i - 2] = str[i++];
//     tmp[i - 2] = '\0';
//     t_val = expand_var(tmp, NULL, env);
//     if (!t_val)
//         return (free(t_val), NULL);
//       i = 0;
//     while (t_val[i])
//         i++;
//     new = malloc(sizeof(char) * (i + 3));
//     i = 1;
//     new[0] = '\'';
//     while (t_val[i - 1])
//     {
//         new[i] = t_val [i - 1];
//         i++;
//     }
//     new[i] = '\'';
//     new[i + 1] ='\0';
//     return(new);
// }

t_token *assign_args(t_token *tokens, t_cmd *cmds, t_env *env)
{
    t_token *current_t;
    t_cmd *current_c;
    int i;
    char *tmp;

    i = 0;
    while(cmds->args[i])
        i++;
    if (tokens && tokens->value && tokens->type == TOKEN_WORD)
    {
        cmds->args[i] = ft_strdup(tokens->value); 
        i++;
    }
    cmds->args[i] = NULL;
    return (tokens);
}

int find_char_pos(char *str, char c)
{
    int i;

    i = 0;

    while (str[i])
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return (0);
}
// char *expand_var_in_str(t_token *token, t_env *env)
// {
//     int i;
//     char *val;
//     char *temp;
//     char *temp2;

//     i = 0;
//     i = find_char_pos(token->value, '$');
           
//             strncpy(temp , token->value, i + 1);
           
//             temp[i] = '\0';
//             strcpy(temp2 + i, token->value);
//     while(env)
//     {
//         if (!strcmp(temp2, env->key))
//         {
//                 val = env->value;
//                 break ;
//         }
//         env = env->next;
//     }
//     if (val)
//     {
//         strcat(temp, val);
//         val = temp;
//         return (val);
//     }
//     return(NULL);
// }

char *expand_var(char *str, t_cmd *cmd, t_env *env)
{
    char *val;
    int i;
    char *temp;
    char *temp2;

    i = 0;
 
    while(env)
    {
        if (!strcmp(str, env->key))
        {
                val = ft_strdup(env->value);
                return(val);
        }
        else if (cmd && str[0] == '?' && !str[1])
                cmd->exit_status = 1;
        env = env->next;
        i++;
    }
    return(NULL);
}
t_token *handle_redirect (t_cmd *cmd, t_token *token, int type)
{
    int token_len;

    token_len = 0;

    
     if (type == TOKEN_REDIRECT_IN)
    {
         if (token->next)
            token = token->next;
        cmd->infile = ft_strdup(token->value);
    }
    else if (type == TOKEN_REDIRECT_OUT)
    {  
        if (token->next)
            token = token->next;
        if (cmd->outfile)
            free(cmd->outfile);
        cmd->outfile = ft_strdup(token->value);
    }
 //  token = token->next;
   return (token);
}

t_token *handle_heredoc (t_cmd * cmd, t_token * token)
{
        cmd->heredoc = 1;
        if (token->next)
            token = token->next;
            cmd->heredoc_delim = ft_strdup(token->value);
        if (token->inside_single || token->inside_double)
            cmd->heredoc_quoted = 1;
            return (token);
}

void handle_varible (t_cmd *cmd, t_token *token, t_env *envp)
{
    char *temp;
    int i;
    char *joined;
    
    i = 0;
        while (cmd->args[i])
                i++;
                if (!token->inside_single && i != 0 && !token->new_word)
                { 
                        temp = expand_var(token->next->value, cmd, envp);
                        if (temp)
                        {
                            joined = ft_strjoin(cmd->args[i - 1], temp);
                            free(cmd->args[i - 1]);
                            cmd->args[i - 1] = joined;
                            free(temp);
                        }
                }
            else if (!token->inside_single)
                cmd->args[i] = expand_var(token->next->value, cmd, envp);         
        else
            cmd->args[i] = token->value;
        i = 0;    
}

t_token *assign_ctl_tokens(t_token *token, t_cmd *cmd, t_env *envp)
{
    int type;
    int i;
    char *temp;

    i = 0;

    type = token->type;
    if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT)
       token =  handle_redirect(cmd, token, type);
    else if (type == TOKEN_HEREDOC)
      token = handle_heredoc(cmd, token);
    else if (type == TOKEN_APPEND)
    {
        cmd->append = 1;
        if (token->next)
            token = token->next;
            cmd->outfile = ft_strdup(token->value);
    }
    else if (type == TOKEN_VARIABLE)
        handle_varible(cmd, token, envp);
    return (token);
}

void shift_left(char **arr)
{
    int i = 0;
    char *tmp;
    
    while (arr[i + 1])
    {
    //    printf("arr[i] in shift = %s\n", arr[i + 1]);
            arr[i] = arr[i + 1];
        i++;
    }
    arr[i] = NULL;
}

t_cmd *handle_pipes(t_cmd *cmds, t_token *tokens, t_env *envp)
{
                if (!cmds->cmd)
                {
                    cmds->cmd = cmds->args[0];
                    shift_left(cmds->args);
                }
            cmds->next = new_cmd_token(tokens, envp);
            cmds = cmds->next;
        return (cmds);
}

// void handle_join (t_cmd *cmds, int i)
// {
//             cmds->args[i - 1] = ft_strjoin(cmds->args[i - 1], cmds->args[i]);
//             cmds->args[i] = NULL;
// }

void handle_join(t_cmd *cmds, int i)
{
    char *joined;
    if (!cmds->args[i - 1] || !cmds->args[i])
        return;

    char *tmp = ft_strjoin(cmds->args[i - 1], cmds->args[i]);
free(cmds->args[i - 1]);
cmds->args[i - 1] = tmp;
free(cmds->args[i]);
cmds->args[i] = NULL;
}

t_token *cmd_loop(t_token *tokens, t_cmd *cmds, int type, t_env *envp)
{ 
    int i;
    while (tokens)
    {
        i = 0;
         type = tokens->type;
         if (type == TOKEN_PIPE)
         cmds = handle_pipes(cmds, tokens, envp);
       else if (type == TOKEN_VARIABLE && !tokens->inside_double && (tokens->next->inside_single || tokens->next->inside_double))
                tokens = tokens->next;
        if (tokens->type == TOKEN_WORD)
            tokens = assign_args(tokens, cmds, envp);
        else
            tokens = assign_ctl_tokens(tokens, cmds, envp);
        while(cmds->args[i])
            i++;
       i = i - 1;
        if (i > 0 && !tokens->new_word && type != TOKEN_VARIABLE)
                handle_join(cmds, i);
        if (tokens->type == TOKEN_VARIABLE)
            tokens= tokens->next;
        tokens = tokens->next;
    }
    return (tokens);
}
t_cmd *init_cmds(t_token *tokens, t_env *envp)
{
    t_cmd *head;
    t_cmd *cmds;
    int i;
    int type;

    i = 0;
    
    cmds = new_cmd_token(tokens, envp);
    head = cmds;
    tokens = cmd_loop(tokens, cmds, type, envp);
    if (!tokens || tokens->new_word)
    {
        while (cmds->next)
            cmds = cmds->next;
        cmds->cmd = cmds->args[0];
        shift_left(cmds->args);
    }
    // print_cmd_list(head);
    return (head);
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
         if (current->heredoc)
		 	printf("[heredoc] ");
        if (current->heredoc_delim)
            printf("heredoc_delim = %s\n", current->heredoc_delim);
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
