/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 10:35:08 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/29 15:34:23 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	child_cleanup_exit(t_shell *shell, t_pipeline *p,
		char *path, int status)
{
	if (path)
		free(path);
	if (p)
		ft_free(p);
	if (shell && shell->envp)
		free_tab(shell->envp);
	exit(status);
}

void	execution(t_pipeline *p, t_shell *shell)
{
	if (heredoc_preparation(p, shell))
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (p->nb_commands == 1)
		single_cmd(&p->commands[0], shell, p);
	else
		multiple_cmd(p, shell);
	ft_setup_interactive_signals();
}

static void	check_execution_error(char *path, t_shell *shell, t_pipeline *p)
{
	struct stat	sb;
	int			err;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Is a directory", 2);
		child_cleanup_exit(shell, p, path, CMD_NOT_EXECUTABLE);
	}
	if (access(path, X_OK) != 0)
	{
		err = errno;
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (err == ENOENT)
			child_cleanup_exit(shell, p, path, CMD_NOT_FOUND);
		child_cleanup_exit(shell, p, path, CMD_NOT_EXECUTABLE);
	}
}

void	child_process(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	char	*path;
	int		err;

	if (!cmd->argv || !cmd->argv[0])
		child_cleanup_exit(shell, p, NULL, 0);
	path = resolve_path(cmd->argv[0], shell->envp);
	if (!path)
	{
		print_cmd_not_found(cmd->argv[0]);
		child_cleanup_exit(shell, p, NULL, CMD_NOT_FOUND);
	}
	ft_setup_child_signals();
	check_execution_error(path, shell, p);
	execve(path, cmd->argv, shell->envp);
	err = errno;
	perror(cmd->argv[0]);
	if (err == ENOENT)
		child_cleanup_exit(shell, p, path, CMD_NOT_FOUND);
	child_cleanup_exit(shell, p, path, CMD_NOT_EXECUTABLE);
}

void	wait_pipeline(pid_t last_pid, t_shell *shell, int n)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	while (i < n)
	{
		pid = wait(&status);
		if (pid == last_pid)
		{
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
		i++;
	}
}
