/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:13 by joshapir          #+#    #+#             */
/*   Updated: 2025/07/20 18:32:57 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    check_quotes(t_token *token, char *str)
{
    int s_quote;
    int d_quote;
    int i;

    s_quote = 0;
    d_quote = 0;
    i = 0;

    while (str[i])
    {
        if (str[i] == '"')
        {
          //  if (str[i + 1])
                i++;
            while (str[i] && str[i] != '"')
            {
                
                // if (s_quote == 1)
                // s_quote = 0;
                // else
                //     s_quote = 1;
                i++;
            }
            if (str[i] != '"' || i == 0)
            {
                 printf("Error: no closing quotes\n");
                return (0);
            }
        }
        else if (str[i] == '\'')
        {
       //     if (str[i + 1])
                i++;
            while (str[i] && str[i] != '\'')
            {
            //     if (d_quote == 1)
            //         d_quote = 0;
            // else
            //     s_quote = 1;
                i++;
            }
            if (str[i] != '\'' || i == 0)
            {
                 printf("Error: no closing quotes\n");
                return (0);
            }
        }
        i++;
    }
    //     while (token)
    // {
    //     if (token->value[0] == '\'')
    //     {
    //         if (s_quote == 1)
    //             s_quote = 0;
    //         else
    //             s_quote = 1;
    //     }
    //     token = token->next;
    // }
    // if (s_quote != 0 || d_quote != 0)
    // {
    //     printf("Error: no closing quotes\n");
    //     return(0);
    // }
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
        {
            printf("Syntax error, invalid token at start\n");
            return (0);
        }
        if (tokens->type == TOKEN_VARIABLE && !tokens->next)
        {
            printf("Syntax error, invalid token at start\n");
            return (0);
        }       
    while (tokens)
    {
        //printf("token = %d\n", tokens->type);
        prev_type = tokens->type;
        if (tokens->type == TOKEN_PIPE)
        {
           // printf("goes in\n");
            prev_type = tokens->type;
            if (!tokens->next || tokens->next->value[0] == '\0')
            {
                printf("Syntax error: pipe without command\n");
                return (0);
            }
            //tokens = tokens->next;
            if (tokens->next->type != TOKEN_WORD && tokens->next->type != TOKEN_VARIABLE)
            {
                printf("Syntax error: pipe without command\n");
                return (0);
            }
             //printf("token type = %d\n", tokens->type);
        }
         else if(tokens->type == TOKEN_HEREDOC || tokens->type == TOKEN_APPEND)
         {
            if (!tokens->next || tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error near unexpected token\n");
                return (0);
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

// int arg_count(t_token *tokens)
// {
//     int count;

//     count = 0;
//     // if (tokens->type == TOKEN_PIPE)
//     //     tokens = tokens->next;
//     while (tokens && tokens-> type == TOKEN_WORD)
//     {
//           count++; 
//           tokens = tokens->next;
//           if (tokens && tokens->type == TOKEN_PIPE)
//                 break ;
//           if (tokens && tokens->type != TOKEN_WORD)
//                 tokens = tokens->next;
//     }
//     return(count);
// }

int arg_count(t_token *tokens)
{
    int count = 0;

    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_WORD)
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
        cmd->args = calloc(101, sizeof(char *));
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

char *expand_with_quotes(char *str, t_env *env)
{
    char *tmp;
    char *t_val;
    char *new;
    int i;
    char quote;
    int count;
    
    count = 0;
    t_val = NULL;
    tmp = NULL;
    quote = str[0];
    i = 0;
    i++;
    while (str[i] != quote)
        i++;
    
    tmp = malloc(sizeof(char) * (i + 1));
    i = 2;
    while(str[i] != quote)
    {
    
        tmp[i - 2] = str[i];
        i++; 
    }
    tmp[i - 2] = '\0';
    
    t_val = expand_var(tmp, NULL, env);
    if (!t_val)
        return (free(t_val), NULL);
      i = 0;
    while (t_val[i])
        i++;
    new = malloc(sizeof(char) * (i + 3));
    i = 1;
    new[0] = '\'';
    while (t_val[i - 1])
    {
        new[i] = t_val [i - 1];
        i++;
    }
    new[i] = '\'';
    new[i + 1] ='\0';
    return(new);
}

t_token *assign_args(t_token *tokens, t_cmd *cmds, t_env *env)
{
    t_token *current_t;
    t_cmd *current_c;
    int i;
    char *tmp;

    i = 0;
    //current_t = *(tokens);
    //current_c = *(cmds); 
    while(cmds->args[i])
        i++;
    // printf("i = %d\n", i);
    // if (ft_strchr(tokens-> value, '$') && !tokens->inside_single && ft_strlen(tokens->value) > 1)
    // {
    //     if (tokens->value[0] == '\'')
    //     {
    //         cmds->args[i] = ft_strdup(expand_with_quotes(tokens->value, env));
    //         if (cmds->args[i] == NULL)
    //         {
    //             cmds->args[i] = ft_strdup("''");
    //         }
    //         //printf("args[i] = %s\n", cmds->args[i]);
    //         i++;
    //     }
    // }
    if (tokens && tokens->type == TOKEN_WORD)
    {
       
       // printf("token->value = %s\n", tokens->value);
    //    if (tokens->value[0] == '\0' && ft_strlen(tokens->value) == 1)
    //         cmds->args[i] = ft_strdup_char('\0');
   
        cmds->args[i] = ft_strdup(tokens->value); 
        i++;
        //tokens = tokens->next;
    }
    cmds->args[i] = NULL;
  //  printf("token type at end of assign = %d\n", tokens->type);
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
char *expand_var_in_str(t_token *token, t_env *env)
{
    int i;
    char *val;
    char *temp;
    char *temp2;

    i = 0;
    i = find_char_pos(token->value, '$');
           
            strncpy(temp , token->value, i + 1);
           
            temp[i] = '\0';
            strcpy(temp2 + i, token->value);
    while(env)
    {
        if (!strcmp(temp2, env->key))
        {
                val = env->value;
                break ;
        }
        env = env->next;
    }
    if (val)
    {
        strcat(temp, val);
        val = temp;
        return (val);
    }
    return(NULL);
}

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
        else if (str[0] == '?' && !str[1])
                cmd->exit_status = 1;
        env = env->next;
        i++;
    }
    return(NULL);
}

t_token *assign_ctl_tokens(t_token *token, t_cmd *cmd, t_env *envp)
{
    int type;
    int i;
    char *temp;

    i = 0;

    type = token->type;
    if (type == TOKEN_REDIRECT_IN)
    {
         if (token->next)
            token = token->next;
        cmd->infile = ft_strdup(token->value);
      //  token = token->next;
    }
    else if (type == TOKEN_REDIRECT_OUT)
    {  
        if (token->next)
            token = token->next;
        if (cmd->outfile)
            free(cmd->outfile);
        cmd->outfile = ft_strdup(token->value);
      //  token = token->next;
    }
    else if (type == TOKEN_HEREDOC)
    {
        cmd->heredoc = 1;
        if (token->next)
            token = token->next;
            cmd->heredoc_delim = ft_strdup(token->value);
        if (token->inside_single || token->inside_double)
            cmd->heredoc_quoted = 1;
      //  token = token->next;
    }
    else if (type == TOKEN_APPEND)
    {
        cmd->append = 1;
        if (token->next)
            token = token->next;
            cmd->outfile = ft_strdup(token->value);
       // token = token->next;
    }
    else if (type == TOKEN_VARIABLE)
    {
        i = 0;
        while (cmd->args[i])
                i++;
   //     printf("len of var %ld\n", strlen(token->value));
        if (!token->inside_single)
        {
            
       //     if (strlen(token->value) == 1)
      //      {   
                if (i != 0 && !token->new_word)
                { 
                    //token = token->next; 
                    temp = expand_var(token->next->value, cmd, envp);
                    if (temp)
                        cmd->args[i - 1] = ft_strjoin(cmd->args[i - 1], temp);
                    //free(temp);
                    // if (!token->next->new_word)
                    // {
                    //     token = token->next;
                    //     cmd->args[i] = ft_strjoin(cmd->args[i], token->value);   
                    // }
       //         } 
     //          else
     //               cmd->args[i] = expand_var(token, envp);
               
                // if (token->next->next && !token->next->next->new_word)
                //         cmd->args[i] = ft_strjoin(cmd->args[i], token->next->next->value);
        //        printf("args = %s\n", cmd->args[i]);
              //  token = token->next;
            }
        /*    else if (ft_strchr(token->value, '$') && !token->inside_single)
            {
                ft_strtrim()
            }
            */
            else
            {
                cmd->args[i] = expand_var(token->next->value, cmd, envp); 
                
            }
        //    else
        //    {
        //        cmd->args[i] = expand_var_in_str(token, envp);
        //        token = token->next;
        //    }
        }
        else
        {
            cmd->args[i] = token->value;
           // token = token->next;
        }
        i = 0;
        // if (token)
        // {
        //     while(cmd->args[i])
        //         i++;
        //         i = i - 1;
        //             if (i != 0 && !token->new_word && cmd->args[i])
        //             {
        //                 //token = token->next;
                        
        //                 cmd->args[i - 1] = ft_strjoin(cmd->args[i - 1], cmd->args[i]);
        //                 free(cmd->args[i]);   
        //             }
        //             i++;
        //             //token = token->next;
        // }
        //token = token->next;
       
    }
    return (token);
}

void shift_left(char **arr)
{
    int i = 0;
    char *tmp;
    
    // while (arr[i])
    //     i++;
    // if (i == 0)
    // {
    //     arr[0] = NULL;
    //   //  arr[1] = NULL;
    // }
    // i = 0;
//    printf("arr[i] = %s\n", arr[0]);
    while (arr[i + 1])
    {
            // if (arr[i + 1])
            //     printf("argss = %s\n", arr[i + 1]);
        // if (arr[i + 1][0] == '\0')
        //     arr[i][0] = '\0';
        // else
            arr[i] = arr[i + 1];
            // printf("i = %d\n", i);
        i++;
    }
    arr[i] = NULL;
    // printf("here\n");
}

t_cmd *init_cmds(t_token *tokens, t_env *envp)
{
    t_cmd *head;
    t_cmd *cmds;
    int i;
    int type;

    i = 0;
    
    cmds = new_cmd_token(tokens, envp);
    //printf("test\n");
    head = cmds;
    // if (tokens->type == TOKEN_WORD && (!ft_strchr(tokens-> value, '$') && !tokens->inside_single))
    // {
    //         tokens = tokens->next;
    // }
    // else if (tokens->type == TOKEN_VARIABLE)
    // {
    //     tokens = tokens->next->next;
    // }
    while (tokens)
    { 
        type = tokens->type;
         if (type == TOKEN_PIPE)
        {
                if (!cmds->cmd)
                {
                    cmds->cmd = cmds->args[0];
                    shift_left(cmds->args);
        
                }
            cmds->next = new_cmd_token(tokens, envp);
            //tokens = tokens->next; not sure what it does
            cmds = cmds->next;
        }
       else if (type == TOKEN_VARIABLE && !tokens->inside_double && (tokens->next->inside_single || tokens->next->inside_double))
                tokens = tokens->next;
        if (tokens->type == TOKEN_WORD)
        {
            tokens = assign_args(tokens, cmds, envp);
        }
        // else if (type == TOKEN_PIPE)
        // {
        //         if (!cmds->cmd)
        //         {
        //             cmds->cmd = cmds->args[0];
        //             shift_left(cmds->args);
        
        //         }
        //     cmds->next = new_cmd_token(tokens, envp);
        //     tokens = tokens->next;
        //     cmds = cmds->next;
        // }
        else
            tokens = assign_ctl_tokens(tokens, cmds, envp);
        i = 0;
        
        while(cmds->args[i])
        {
            // printf("args = %s\n", cmds->args[i]);   
            i++;
        }
        i = i - 1;
       // printf("args value here is %s\n", cmds->args[i]);
        if (i > 0 && !tokens->new_word && type != TOKEN_VARIABLE)
        {
            cmds->args[i - 1] = ft_strjoin(cmds->args[i - 1], cmds->args[i]);
            cmds->args[i] = NULL;
        }
        if (tokens->type == TOKEN_VARIABLE)
            tokens= tokens->next;
        tokens = tokens->next;
        // if (!cmds->cmd)
        // {
        //     printf("args[0] = %s\n", cmds->args[0]);
        //     cmds->cmd = ft_strdup(cmds->args[0]);
        //     free(cmds->args[0]);
        //     cmds->args[0] = NULL;
        // }
    //     if (!cmds->cmd && (tokens && tokens->new_word || !tokens))
    // {
    //     cmds->cmd = cmds->args[0];
    //     // if (cmds->args[1])
    //     //     free(cmds->args[1]);

    //     shift_left(cmds->args);
        
    // }
        
    }
    if (!tokens || tokens->new_word)
    {
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
