/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:09:31 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/17 15:57:30 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	handler_builtin(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	char	*cmd_name;

	if (!cmd->argv || !cmd->argv[0])
		return (-1);
	cmd_name = cmd->argv[0];
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (cmd_echo(cmd));
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (cmd_pwd());
	if (ft_strcmp(cmd_name, "env") == 0)
		return (cmd_env(shell));
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (cmd_cd(cmd, shell));
	if (ft_strcmp(cmd_name, "export") == 0)
		return (cmd_export(cmd, shell));
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (cmd_unset(cmd, shell));
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (cmd_exit(cmd, shell, p));
	return (-1);
}
