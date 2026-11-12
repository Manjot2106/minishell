/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:03:30 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/18 14:39:28 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	**search_path(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

static char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp_path;
	char	*full_path;

	tmp_path = ft_strjoin(dir, "/");
	if (!tmp_path)
		return (NULL);
	full_path = ft_strjoin(tmp_path, cmd);
	free(tmp_path);
	if (!full_path)
		return (NULL);
	return (full_path);
}

static char	*add_slash_cmd(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	size_t	i;

	paths = search_path(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tab(paths);
	return (NULL);
}

char	*resolve_path(char *cmd, char **envp)
{
	char	*path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = add_slash_cmd(cmd, envp);
	return (path);
}
