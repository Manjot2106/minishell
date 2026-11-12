/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:06:51 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 14:03:04 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	export_print(t_shell *shell);

static char	*get_key(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (ft_substr(str, 0, i));
}

static void	add_env_var(char *arg, t_shell *shell)
{
	char	**new_env;
	int		i;

	i = 0;
	while (shell->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = 0;
	while (shell->envp[i])
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
}

static void	export_add(char *arg, t_shell *shell)
{
	char	*key;
	int		index;

	key = get_key(arg);
	index = env_index(shell->envp, key);
	if (index >= 0)
	{
		free(shell->envp[index]);
		shell->envp[index] = ft_strdup(arg);
	}
	else
		add_env_var(arg, shell);
	free(key);
}

int	cmd_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	if (!cmd->argv[1])
		return (export_print(shell));
	i = 1;
	while (cmd->argv[i])
	{
		if (!export_valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: export: '", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
		}
		else
			export_add(cmd->argv[i], shell);
		i++;
	}
	return (status);
}
