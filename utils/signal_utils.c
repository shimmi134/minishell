
#include "../minishell.h"
void	handle_sigint(int sig_num)
{
	(void)sig_num;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	g_exit_code = 130;
}