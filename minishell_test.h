#ifndef MINISHELL_TEST_H
# define MINISHELL_TEST_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_shell
{
	char			*type;
	char			*word;
	struct s_shell	*next;
}					t_shell;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;


char				**ft_split(char const *s, char c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
size_t				ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
#endif
