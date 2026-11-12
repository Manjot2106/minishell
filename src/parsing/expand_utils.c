/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:45:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/16 12:12:52 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	append_str(t_bufpair *bp, const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (bufpair_push(bp, s[i], '0') < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	append_status(t_bufpair *bp, int last_status)
{
	char	*status;
	int		rc;

	status = ft_itoa(last_status);
	if (!status)
		return (-1);
	rc = append_str(bp, status);
	free(status);
	return (rc);
}

static char	*find_env_value(char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	expand_dollar(t_bufpair *bp, const char *s, int *i, char **envp)
{
	int		start;
	char	*name;
	char	*val;

	if (ft_isdigit(s[*i + 1]))
		return (*i += 2, 0);
	if (!(ft_isalpha(s[*i + 1]) || s[*i + 1] == '_'))
		return ((*i)++, bufpair_push(bp, '$', '0'));
	start = *i + 1;
	*i = start;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_substr(s, start, *i - start);
	if (!name)
		return (-1);
	val = find_env_value(name, envp);
	free(name);
	if (!val)
		return (0);
	return (append_str(bp, val));
}
