/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:06:47 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/20 14:30:05 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_overflow_ll(char *arg)
{
	int					i;
	unsigned long long	n;
	unsigned long long	limit;

	i = 0;
	n = 0;
	if (arg[i] == '-')
		limit = 9223372036854775808ULL;
	else
		limit = 9223372036854775807ULL;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		n = n * 10 + (arg[i] - '0');
		if (n > limit)
			return (1);
		i++;
	}
	return (0);
}

static int	exit_status_value(char *arg)
{
	int	mod;
	int	i;
	int	neg;

	mod = 0;
	i = 0;
	neg = 0;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			neg = 1;
		i++;
	}
	while (arg[i])
	{
		mod = (mod * 10 + (arg[i] - '0')) % 256;
		i++;
	}
	if (neg)
		return ((256 - mod) % 256);
	return (mod);
}

static void	exit_numeric_error(char *arg, t_shell *shell, t_pipeline *p)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_all_before_exit(shell, p, 2);
}

int	cmd_exit(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 1);
	if (!cmd->argv[1])
		free_all_before_exit(shell, p, shell->last_exit_status);
	if (!is_numeric(cmd->argv[1]) || is_overflow_ll(cmd->argv[1]))
		exit_numeric_error(cmd->argv[1], shell, p);
	if (cmd->argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		shell->last_exit_status = 1;
		return (1);
	}
	free_all_before_exit(shell, p, exit_status_value(cmd->argv[1]));
	return (0);
}
