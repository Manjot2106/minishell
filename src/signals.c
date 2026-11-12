/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/29 12:36:49 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status;

void	ft_set_signal_status(int status)
{
	g_signal_status = status;
}

int	ft_get_signal_status(void)
{
	return (g_signal_status);
}

static void	interactive_sigint(int signo)
{
	(void)signo;
	ft_set_signal_status(130);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_setup_interactive_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, interactive_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
