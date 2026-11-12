/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 00:00:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/25 14:22:41 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	heredoc_sigint(int signo)
{
	(void)signo;
	ft_set_signal_status(130);
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

int	ft_heredoc_signals_enter(void)
{
	int	stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (-1);
	ft_set_signal_status(0);
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (stdin_backup);
}

int	ft_heredoc_signals_leave(int stdin_backup)
{
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
	{
		close(stdin_backup);
		ft_setup_interactive_signals();
		return (-1);
	}
	close(stdin_backup);
	ft_setup_interactive_signals();
	return (0);
}

int	ft_heredoc_interrupted(void)
{
	return (ft_get_signal_status() == 130);
}
