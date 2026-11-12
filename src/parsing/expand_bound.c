/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_bound.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:45:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/16 12:12:52 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	same_quote_end(t_token *t, int i)
{
	int	j;

	j = i;
	if (!t->qstate)
		return (j + ft_strlen(t->value + j));
	while (t->value[j] && t->qstate[j] == t->qstate[i])
		j++;
	return (j);
}

int	expand_bounded(t_token *t, t_bufpair *bp, int *i, char **envp)
{
	int		end;
	char	saved;
	int		rc;

	end = same_quote_end(t, *i);
	saved = t->value[end];
	t->value[end] = '\0';
	rc = expand_dollar(bp, t->value, i, envp);
	t->value[end] = saved;
	return (rc);
}
