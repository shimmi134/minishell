#include "minishell.h"

int has_token(char *str)
{
	int i;

	i = 0;

	while (str[i])
	{
		if (is_token(str[i]))
			return(1);
		i++;
	}
	return(0);
}
