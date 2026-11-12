/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 00:00:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/28 11:07:51 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "parsing.h"

static int	expand_exit_status(t_bufpair *bp, char *line, int *i, int status)
{
	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		if (append_status(bp, status) < 0)
			return (-1);
		*i += 2;
		return (1);
	}
	return (0);
}

static int	expand_heredoc_dollar(t_bufpair *bp, char *line, int *i,
			char **envp)
{
	if (line[*i] != '$')
		return (0);
	if (expand_dollar(bp, line, i, envp) < 0)
		return (-1);
	return (1);
}

void	ft_warn_heredoc_eof(char *delim, int line_no)
{
	char	*line_str;

	line_str = ft_itoa(line_no);
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("at line ", STDERR_FILENO);
	if (line_str)
		ft_putstr_fd(line_str, STDERR_FILENO);
	else
		ft_putstr_fd("?", STDERR_FILENO);
	ft_putstr_fd(" ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	free(line_str);
}

char	*ft_expand_heredoc_line(char *line, t_shell *shell)
{
	t_bufpair	bp;
	int			i;
	int			rc;

	if (bufpair_init(&bp) < 0)
		return (NULL);
	i = 0;
	while (line[i])
	{
		rc = expand_exit_status(&bp, line, &i, shell->last_exit_status);
		if (rc < 0)
			return (free(bp.out), free(bp.st), NULL);
		if (rc > 0)
			continue ;
		rc = expand_heredoc_dollar(&bp, line, &i, shell->envp);
		if (rc < 0)
			return (free(bp.out), free(bp.st), NULL);
		if (rc > 0)
			continue ;
		if (bufpair_push(&bp, line[i++], '0') < 0)
			return (free(bp.out), free(bp.st), NULL);
	}
	free(bp.st);
	return (bp.out);
}
