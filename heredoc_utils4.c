
#include "minishell.h"

void assign_hd_struct_values(t_heredoc **heredoc, t_cmd *cmd)
{
	(*heredoc)->heredoc_delim = ft_strdup(cmd->heredoc_delim[0]);
	(*heredoc)->heredoc_fd = cmd->heredoc_fd;
	(*heredoc)->heredoc_quoted = cmd->heredoc_quoted;
	(*heredoc)->next = NULL;
}
