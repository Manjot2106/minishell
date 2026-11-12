/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:13:51 by msingh            #+#    #+#             */
/*   Updated: 2026/02/27 13:54:12 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	count_cmds(t_token *t)
{
	int	c;

	c = 1;
	while (t)
	{
		if (t->type == PIPE)
			c++;
		t = t->next;
	}
	return (c);
}

static int	argvb_grow(t_argvb *b)
{
	char	**n;
	int		i;

	if (b->len + 1 < b->cap)
		return (0);
	n = ft_calloc(b->cap * 2, sizeof(char *));
	if (!n)
		return (-1);
	i = -1;
	while (++i < b->len)
		n[i] = b->v[i];
	free(b->v);
	b->v = n;
	b->cap *= 2;
	return (0);
}

int	argvb_push(t_argvb *b, char *s)
{
	if (argvb_grow(b) < 0)
		return (-1);
	b->v[b->len++] = ft_strdup(s);
	if (!b->v[b->len - 1])
		return (-1);
	b->v[b->len] = NULL;
	return (0);
}

int	argvb_init(t_argvb *b)
{
	b->cap = 8;
	b->len = 0;
	b->v = ft_calloc(b->cap, sizeof(char *));
	if (!b->v)
		return (-1);
	b->v[0] = NULL;
	return (0);
}

void	argvb_free(t_argvb *b)
{
	int	i;

	if (!b || !b->v)
		return ;
	i = 0;
	while (b->v[i])
		free(b->v[i++]);
	free(b->v);
	b->v = NULL;
}
