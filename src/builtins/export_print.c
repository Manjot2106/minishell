/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:45:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/16 12:08:04 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static char	**copy_env(char **envp)
{
	char	**copy;
	int		size;
	int		i;

	size = env_size(envp);
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		copy[i] = envp[i];
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *line)
{
	char	*equal;

	equal = ft_strchr(line, '=');
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	if (!equal)
		return (ft_putendl_fd(line, STDOUT_FILENO));
	write(STDOUT_FILENO, line, equal - line + 1);
	ft_putstr_fd("\"", STDOUT_FILENO);
	ft_putstr_fd(equal + 1, STDOUT_FILENO);
	ft_putendl_fd("\"", STDOUT_FILENO);
}

int	export_print(t_shell *shell)
{
	char	**sorted;
	int		i;

	sorted = copy_env(shell->envp);
	if (!sorted)
		return (1);
	sort_env(sorted);
	i = 0;
	while (sorted[i])
	{
		print_export_line(sorted[i]);
		i++;
	}
	free(sorted);
	return (0);
}
