/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:47:50 by msingh            #+#    #+#             */
/*   Updated: 2026/03/28 11:31:46 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	token_add_back(t_token **toks, t_token *new)
{
	t_token	*cur;

	if (!toks || !new)
		return ;
	if (!*toks)
	{
		*toks = new;
		return ;
	}
	cur = *toks;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

t_token	*token_new(t_token_type type, char *value, int qmask, char *qstate)
{
	t_token	*t;

	t = ft_calloc(1, sizeof(*t));
	if (!t)
		return (NULL);
	t->type = type;
	t->value = value;
	t->qmask = qmask;
	t->qstate = qstate;
	t->next = NULL;
	return (t);
}

static int	add_word_token(t_token **toks, t_word *w, t_parse_err *err)
{
	t_token	*new;

	new = token_new(WORD, w->str, w->qmask, w->qstate);
	if (!new)
	{
		free(w->str);
		free(w->qstate);
		*err = P_ERR_MALLOC;
		return (-1);
	}
	token_add_back(toks, new);
	return (0);
}

static int	token_next(t_token **toks, char *line, int *i, t_parse_err *err)
{
	t_word	w;

	while (line[*i] && is_space(line[*i]))
		(*i)++;
	if (!line[*i])
		return (0);
	if (line[*i] == '#')
		return (0);
	if (is_meta(line[*i]))
	{
		*i = ft_push(toks, line, *i, err);
		if (*i < 0 || *err != P_OK)
			return (-1);
		return (1);
	}
	ft_bzero(&w, sizeof(w));
	if (ft_read_word(line, i, &w, err) < 0)
		return (-1);
	if (add_word_token(toks, &w, err) < 0)
		return (-1);
	return (1);
}

t_token	*ft_tokeniser(char *line, t_parse_err *err)
{
	t_token	*toks;
	int		i;
	int		r;

	toks = NULL;
	i = 0;
	*err = P_OK;
	while (line && line[i])
	{
		r = token_next(&toks, line, &i, err);
		if (r < 0)
			return (NULL);
		if (r == 0)
			break ;
	}
	return (toks);
}
