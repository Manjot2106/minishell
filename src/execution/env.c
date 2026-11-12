/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:25:16 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/28 11:07:51 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	get_env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	add_env_entry(char *line, t_shell *shell)
{
	char	**new_env;
	int		size;
	int		i;

	size = get_env_size(shell->envp);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < size)
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[i] = line;
	new_env[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
	return (0);
}

void	init_mini(char **envp, t_shell *shell)
{
	int	i;
	int	size;

	size = get_env_size(envp);
	shell->envp = malloc(sizeof(char *) * (size + 1));
	if (!shell->envp)
	{
		perror("malloc");
		exit(1);
	}
	i = 0;
	while (i < size)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			shell->envp[i] = NULL;
			free_tab(shell->envp);
			exit(1);
		}
		i++;
	}
	shell->envp[i] = NULL;
	shell->last_exit_status = 0;
	update_shlvl(shell);
}

char	*get_env_value(char *key, char **envp)
{
	int		i;
	size_t	len;

	if (!key || !envp)
		return (NULL);
	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	update_env(char *key, char *value, t_shell *shell)
{
	int		idx;
	char	*new_line;
	char	*tmp;

	if (!key || !value || !shell->envp)
		return ;
	idx = env_index(shell->envp, key);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return ;
	new_line = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_line)
		return ;
	if (idx >= 0)
	{
		free(shell->envp[idx]);
		shell->envp[idx] = new_line;
		return ;
	}
	if (add_env_entry(new_line, shell) != 0)
		free(new_line);
}
