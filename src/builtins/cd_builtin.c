/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:06:31 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 14:03:29 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*get_cd_path(t_command *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->argv[1])
	{
		path = get_env_value("HOME", shell->envp);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		path = cmd->argv[1];
	return (path);
}

static int	set_old_pwd(char *old_pwd)
{
	if (!getcwd(old_pwd, PATH_MAX))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	update_pwd_env(char *old_pwd, t_shell *shell)
{
	char	new_pwd[PATH_MAX];

	update_env("OLDPWD", old_pwd, shell);
	if (getcwd(new_pwd, PATH_MAX))
		update_env("PWD", new_pwd, shell);
}

int	cmd_cd(t_command *cmd, t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	*path;

	if (cmd->argv[1] && cmd->argv[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (set_old_pwd(old_pwd))
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		perror("getcwd");
		return (1);
	}
	path = get_cd_path(cmd, shell);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		return (1);
	}
	update_pwd_env(old_pwd, shell);
	return (0);
}
