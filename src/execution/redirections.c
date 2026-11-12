/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:52:00 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 14:02:31 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_redirs(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == HEREDOC)
		fd = redir->fd;
	else
		fd = -1;
	return (fd);
}

int	handle_redirs(t_command *cmd)
{
	t_redir	*cur;
	int		fd;

	cur = cmd->redirs;
	while (cur)
	{
		fd = open_redirs(cur);
		if (fd == -1)
			return (perror(cur->target), 1);
		if (cur->type == REDIR_IN || cur->type == HEREDOC)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (error_close("dup2", fd));
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (error_close("dup2", fd));
		}
		close(fd);
		cur = cur->next;
	}
	return (0);
}
