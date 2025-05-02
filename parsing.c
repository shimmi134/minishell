#include "minishell.h"

void check_tokens (t_token *tokens)
{
    t_token *head;
    int prev_type;

    head = tokens;
if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT)
        {
            printf("Syntax error, invalid token at start\n");
            return ;
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
                break ;
            }
            //tokens = tokens->next;
            if (tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error: pipe without command\n");
                break ;
            }
             //printf("token type = %d\n", tokens->type);
        }
        else if (tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT)
        {
            if (!tokens->next)
            {
                printf("doesnt exist\n");
                printf("Syntax error: expected filename\n");
                break ;
            }
            //printf("goes in\n");
            else if ((tokens->next->type) && tokens->next->type != TOKEN_WORD)
            {
                printf("Syntax error: expected filename\n");
               // exit(EXIT_FAILURE);
               break ;
            }
        }
         tokens = tokens->next;
    }
 //   return (1);
}