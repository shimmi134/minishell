/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 03:06:13 by joshapir          #+#    #+#             */
/*   Updated: 2025/05/21 12:25:43 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_cmd *new_cmd_token(t_token *tokens, t_env *envp)
{
    int count;
    
    t_cmd *cmd = malloc(sizeof(t_cmd));
    count = arg_count(tokens) - 1;
        cmd->args = malloc(sizeof(char *) * (count + 1));
    if (!cmd->args)
        return(NULL);
    if (tokens->type == TOKEN_WORD)
        cmd->cmd = tokens->value;
    else if (tokens->type == TOKEN_HEREDOC || tokens->type == TOKEN_APPEND)
            cmd->cmd = NULL;
    else if(!cmd->cmd && !cmd->next && tokens->type != TOKEN_PIPE)
            cmd->cmd = tokens->value;
    else if (tokens->type == TOKEN_VARIABLE && tokens->next->type == TOKEN_WORD)
    {
        cmd->cmd = expand_var(tokens, envp);
    }
    else
        cmd->cmd = tokens->next->value;
        int i;

        i = 0;
    // while (i < count + 1)
    //     cmd->args[i++] = NULL;
    cmd->args[0] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->heredoc_delim = NULL;
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
    if (tokens && tokens->type == TOKEN_WORD)
    {
        while(cmds->args && cmds->args[i])
            i++;
        //printf("token->value = %s\n", tokens->value);
        if (cmds->args)
        	cmds->args[i] = tokens->value;
        i++;
        //tokens = tokens->next;
        if (cmds->args)
        	cmds->args[i] = NULL;
    }
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
            printf("temp value = %s\n", temp2);
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
        printf("vallll = %s\n", val);
        return (val);
    }
    return(NULL);
}

char *expand_var(t_token *token, t_env *env)
{
    char *val;
    int i;
    char *temp;
    char *temp2;

    i = 0;
 //   if (token->inside_single)
   //     return(token->value);
    
 //   if (token->value[0] == '$' && strlen(token->value) > 1)
  //      memmove(token->value, token->value + 1, strlen(token->value));
  //  else 
      //  if (strlen(token->value) == 1)
      //      token = token->next;
        /*
        else
        {
            i = find_char_pos(token->value, '$');
            strncpy(temp , token->value, i);
            temp[i] = '\0';
            strcpy(temp2 + i, token->value);
            printf("temp value = %s\n", temp2);
           // strcat(temp, token->value + i);
           
        }
*/


       /*
        else if (token->value[i] == '\'' || token->value[i] == '\"' || token->value[i] == '$')
        {
            printf("goes in\n");
            while (token->value[i] == '\'' || token->value[i] == '\"' || token->value[i] == '$')
                        i++;
                memmove(token->value, token->value + i, strlen(token->value + i));
        }
        */
    while(env)
    {
        if (!strcmp(token->value, env->key))
        {
                val = env->value;
                return(val);
        }
        env = env->next;
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
        cmd->outfile = ft_strdup(token->value);
      //  token = token->next;
    }
    else if (type == TOKEN_HEREDOC)
    {
        cmd->heredoc = 1;
        if (token->next)
            token = token->next;
            cmd->heredoc_delim = ft_strdup(token->value);
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
                    temp = expand_var(token->next, envp);
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
            else
                cmd->args[i] = expand_var(token->next, envp); 
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
t_cmd* init_cmds(t_token *tokens, t_env *envp)
{
    t_cmd *head;
    t_cmd *cmds;
    int i;
    int type;

    i = 0;
    
    cmds = new_cmd_token(tokens, envp);
    //printf("test\n");
    head = cmds;
    if (tokens->type == TOKEN_WORD)
    {
            tokens = tokens->next;
    }
    else if (tokens->type == TOKEN_VARIABLE)
    {
        tokens = tokens->next->next;
    }
    while (tokens)
    { 
        type = tokens->type;
        if (type == TOKEN_WORD)
            tokens = assign_args(tokens, cmds);
        else if (type == TOKEN_PIPE)
        {
			t_cmd *helper = malloc(sizeof(t_cmd));
			helper->pipe = 1;
			helper->cmd = NULL;
			helper->args = NULL;
			helper->infile = NULL;
			helper->outfile = NULL;
			helper->append = 0;
			helper->heredoc = 0;
			helper->heredoc_delim = NULL;
			cmds->next = helper;
			cmds = cmds->next;
			/*
            cmds->next = new_cmd_token(tokens, envp);
            tokens = tokens->next;
            cmds = cmds->next;
			*/
        }
        else
            tokens = assign_ctl_tokens(tokens, cmds, envp);
        i = 0;
        while(cmds->args && cmds->args[i])
            i++;
        i = i - 1;
        if (i != 0 && !tokens->new_word && type != TOKEN_VARIABLE)
        {
            cmds->args[i - 1] = ft_strjoin(cmds->args[i - 1], cmds->args[i]);
            cmds->args[i] = NULL;
        }
        if (tokens->type == TOKEN_VARIABLE)
            tokens= tokens->next;
        tokens = tokens->next;
        
    }
	return head;
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
            printf("cmd = %s\n", current->cmd);
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
