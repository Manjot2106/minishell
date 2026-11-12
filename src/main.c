/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 11:07:59 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 15:43:01 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parsing.h"

static void	handle_parsed_line(t_shell *shell, char *line)
{
	t_pipeline	*pipeline;
	int			parse_status;

	if (*line)
		add_history(line);
	pipeline = parse_line(line, shell->last_exit_status, shell->envp,
			&parse_status);
	if (pipeline)
	{
		execution(pipeline, shell);
		ft_free(pipeline);
	}
	else if (parse_status != 0)
		shell->last_exit_status = parse_status;
}

static void	minishell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		shell->last_exit_status = ft_consume_signal_status
			(shell->last_exit_status);
		handle_parsed_line(shell, line);
		free(line);
	}
}

int	ft_consume_signal_status(int fallback)
{
	if (ft_get_signal_status() != 0)
	{
		fallback = ft_get_signal_status();
		ft_set_signal_status(0);
	}
	return (fallback);
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac != 1)
		return (-1);
	init_mini(env, &shell);
	ft_setup_interactive_signals();
	minishell_loop(&shell);
	free_tab(shell.envp);
	rl_clear_history();
	return (shell.last_exit_status);
}
