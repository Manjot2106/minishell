/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:51:07 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/17 12:37:33 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	setup_child_fds(t_exec_state *state, int i)
{
	if (state->prev_fd != -1)
	{
		if (dup2(state->prev_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(state->prev_fd);
	}
	if (i < state->p->nb_commands - 1)
	{
		close(state->fd[0]);
		if (dup2(state->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(state->fd[1]);
	}
	if (handle_redirs(&state->p->commands[i]) != 0)
		exit(EXIT_FAILURE);
}

static void	child_pipeline(t_exec_state *state, int i)
{
	int	status;

	setup_child_fds(state, i);
	status = handler_builtin(&state->p->commands[i], state->shell, state->p);
	if (status != -1)
	{
		exit(status);
	}
	child_process(&state->p->commands[i], state->shell, state->p);
}

static void	parent_pipeline(t_exec_state *state, int i)
{
	if (state->prev_fd != -1)
	{
		close(state->prev_fd);
		state->prev_fd = -1;
	}
	if (i < state->p->nb_commands - 1)
	{
		close(state->fd[1]);
		state->prev_fd = state->fd[0];
	}
}

static pid_t	fork_step(t_exec_state *state, int i)
{
	pid_t	pid;

	if (i < state->p->nb_commands - 1)
	{
		if (pipe(state->fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		child_pipeline(state, i);
	parent_pipeline(state, i);
	return (pid);
}

pid_t	fork_pipeline(t_exec_state *state)
{
	int		i;
	pid_t	pid;
	pid_t	last_pid;

	i = 0;
	last_pid = -1;
	while (i < state->p->nb_commands)
	{
		pid = fork_step(state, i);
		if (pid == -1)
			return (-1);
		if (i == state->p->nb_commands - 1)
			last_pid = pid;
		i++;
	}
	if (state->prev_fd != -1)
		close(state->prev_fd);
	return (last_pid);
}
