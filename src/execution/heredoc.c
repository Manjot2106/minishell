/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:45:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/28 11:07:51 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	write_heredoc_line(int fd, char *line, int quoted, t_shell *shell)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = ft_expand_heredoc_line(line, shell);
		if (!expanded)
			return (free(line), -1);
		free(line);
		line = expanded;
	}
	ft_putstr_fd(line, fd);
	write(fd, "\n", 1);
	free(line);
	return (0);
}

static int	heredoc_end(int stdin_backup, int status, char *delim, int line_no)
{
	if (ft_heredoc_signals_leave(stdin_backup) == -1)
		return (-1);
	if (status == 2)
		ft_warn_heredoc_eof(delim, line_no);
	return (status);
}

static int	heredoc_read(int *fd, char *delim, int quoted, t_shell *shell)
{
	char	*line;
	int		stdin_backup;
	int		line_no;

	stdin_backup = ft_heredoc_signals_enter();
	if (stdin_backup == -1)
		return (-1);
	line_no = 1;
	while (1)
	{
		line = readline("> ");
		if (ft_heredoc_interrupted())
			return (free(line), heredoc_end(stdin_backup, 1, delim, line_no));
		if (!line)
			return (heredoc_end(stdin_backup, 2, delim, line_no));
		if (ft_strcmp(line, delim) == 0)
			return (free(line), heredoc_end(stdin_backup, 0, delim, line_no));
		if (write_heredoc_line(fd[1], line, quoted, shell) < 0)
			return (heredoc_end(stdin_backup, -1, delim, line_no));
		line_no++;
	}
}

int	execute_heredoc(char *delim, int quoted, t_shell *shell)
{
	int	fd[2];
	int	read_status;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	read_status = heredoc_read(fd, delim, quoted, shell);
	if (read_status == 2)
	{
		close(fd[1]);
		return (fd[0]);
	}
	if (read_status != 0)
	{
		close(fd[0]);
		close(fd[1]);
		if (read_status < 0)
			return (ft_putendl_fd("minishell: memory allocation error", 2), -1);
		shell->last_exit_status = 130;
		return (-1);
	}
	close(fd[1]);
	return (fd[0]);
}

int	heredoc_preparation(t_pipeline *p, t_shell *shell)
{
	int		i;
	t_redir	*redir;

	i = 0;
	while (i < p->nb_commands)
	{
		redir = p->commands[i].redirs;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				redir->fd = execute_heredoc
					(redir->target, redir->quoted, shell);
				if (redir->fd == -1)
					return (-1);
			}
			redir = redir->next;
		}
		i++;
	}
	return (0);
}
