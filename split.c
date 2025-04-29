#include "minishell.h"

int ft_word_count(char *str)
{
    int i;
    int count;
    int flag;

    i = 0;
    count = 0;
    flag = 0;

    while(str[i])
    {
        while(str[i] == ' ')
        {
           // flag = 0;
            i++;
        }
        if ((str[i]) && str[i] != ' ')
                count++;
        while((str[i]) && str[i] != ' ')
        {
            i++;
        }
    }
   // printf("word count = %d\n", count);
    return(count);
}

int ft_letter_count(char *str, int i)
{
    int count;

    count = 0;

    while ((str[i]) && str[i] == ' ')
         i++;
    while ((str[i]) && str[i] != ' ')
    {
        count++;
        i++;
    }
   // printf("letter count = %d\n", count);
    return (count);
}
 int   ft_next_word(char *str, int i)
{
    while ((str[i]) && (str[i] != ' '))
            i++;
    while((str[i]) && str[i] == ' ')
        i++;
    return(i);
}
char **ft_split(char *str)
{
    char **arr;
    int i;
    int j;
    int word_count;

    i = 0;
    j = 0;
    word_count = ft_word_count(str);
    arr = malloc(sizeof(char *) * (word_count + 1));
    if ((str[i]) && str[i] == ' ')
        i = ft_next_word(str, i);
    while (j < word_count)
    {
        int letter_count = ft_letter_count(str, i);
        arr[j] = malloc(sizeof(char) * letter_count + 1);
        i = ft_next_word(str, i);
        j++;
    }
    arr[j] = NULL;
    i = 0;
    j = 0;
    int k = 0;

    if(str[i] == ' ')
        i = ft_next_word(str, i);
    while(str[i])
    {
        while ((str[i]) && str[i] != ' ')
        {
                arr[j][k] = str[i];
                i++;
                k++;
        }
        arr[j][k] = '\0';
        i = ft_next_word(str, i);
        if ((str[i]) && j < word_count)
        {
            j++;
            k = 0;
        }
    }
    return(arr);
}
/*
int main (void)
{
    char *str = "  both sides  ";
    char **arr;
    int i = 0;
    arr = ft_split(str);

    while(arr[i])
    {
        printf("%s\n", arr[i]);
        i++;
    }
    i = 0;
    while(arr[i])
    {
        free(arr[i]);
        i++;
    }
    free (arr);
}
*/
/*
int main(void)
{
    char *test_cases[] = {
        "",
        "   ",
        "hello",
        "hello world",
        "  leading space",
        "trailing space   ",
        "  both sides  ",
        "multiple   spaces here",
        "    a b   c   ",
        NULL
    };

    for (int i = 0; test_cases[i]; i++)
    {
        printf("Test case %d: \"%s\"\n", i + 1, test_cases[i]);
        char **result = ft_split(test_cases[i]);

        int j = 0;
        while (result && result[j])
        {
            printf("[%s]\n", result[j]);
            free(result[j]);
            j++;
        }
        free(result);
        printf("----\n");
    }

    return 0;
}
*/
