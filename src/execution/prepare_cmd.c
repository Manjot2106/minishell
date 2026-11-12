/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 11:22:33 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/18 14:39:28 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	run_single_child(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	int	status;

	if (handle_redirs(cmd) != 0)
		free_all_before_exit(shell, p, EXIT_FAILURE);
	status = handler_builtin(cmd, shell, p);
	if (status != -1)
		free_all_before_exit(shell, p, status);
	child_process(cmd, shell, p);
}

static void	wait_single_child(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

void	single_cmd(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	pid_t	pid;
	int		status;

	if (cmd->redirs == NULL)
	{
		status = handler_builtin(cmd, shell, p);
		if (status != -1)
		{
			shell->last_exit_status = status;
			return ;
		}
	}
	pid = fork();
	if (pid < 0)
		return (perror("fork"));
	if (pid == 0)
		run_single_child(cmd, shell, p);
	wait_single_child(pid, shell);
}

void	multiple_cmd(t_pipeline *p, t_shell *shell)
{
	t_exec_state	state;
	pid_t			last_pid;

	state.p = p;
	state.shell = shell;
	state.prev_fd = -1;
	last_pid = fork_pipeline(&state);
	if (last_pid == -1)
		return ;
	wait_pipeline(last_pid, shell, p->nb_commands);
}
