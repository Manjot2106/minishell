/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buf_quote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:40:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/12 11:58:57 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	grow_buf(char **buf, size_t *cap, size_t need)
{
	char	*n;

	if (need <= *cap)
		return (0);
	while (*cap < need)
		*cap *= 2;
	n = ft_calloc(*cap, 1);
	if (!n)
		return (-1);
	ft_memcpy(n, *buf, ft_strlen(*buf));
	free(*buf);
	*buf = n;
	return (0);
}

int	bufpair_init(t_bufpair *bp)
{
	bp->cap_out = 32;
	bp->cap_st = 32;
	bp->len = 0;
	bp->out = ft_calloc(bp->cap_out, 1);
	bp->st = ft_calloc(bp->cap_st, 1);
	if (!bp->out || !bp->st)
	{
		free(bp->out);
		free(bp->st);
		bp->out = NULL;
		bp->st = NULL;
		return (-1);
	}
	return (0);
}

int	bufpair_push(t_bufpair *bp, char c, char q)
{
	if (grow_buf(&bp->out, &bp->cap_out, bp->len + 2) < 0)
		return (-1);
	if (grow_buf(&bp->st, &bp->cap_st, bp->len + 2) < 0)
		return (-1);
	bp->out[bp->len] = c;
	bp->st[bp->len] = q;
	bp->len++;
	bp->out[bp->len] = '\0';
	bp->st[bp->len] = '\0';
	return (0);
}

char	qchar_from_quote(char quote)
{
	if (quote == '\'')
		return ('S');
	if (quote == '"')
		return ('D');
	return ('0');
}

int	handle_quote(char *line, int *i, char *quote, int *qmask)
{
	if (*quote == 0 && (line[*i] == '\'' || line[*i] == '"'))
	{
		if (line[*i] == '\'')
			*qmask |= Q_SQUOTE;
		else
			*qmask |= Q_DQUOTE;
		*quote = line[*i];
		(*i)++;
		return (1);
	}
	if (*quote != 0 && line[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}
