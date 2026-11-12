/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:11:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/12 11:55:52 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_redir	*redir_new(t_token_type type, const char *target, int quoted)
{
	t_redir	*r;

	r = malloc(sizeof(*r));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = ft_strdup(target);
	if (!r->target)
		return (free(r), NULL);
	r->quoted = quoted;
	r->fd = -1;
	r->next = NULL;
	return (r);
}

static void	redir_add_back(t_redir **lst, t_redir *newr)
{
	t_redir	*cur;

	if (!newr)
		return ;
	if (!*lst)
	{
		*lst = newr;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = newr;
}

int	redir_push_from_token(t_command *cmd, t_token *t)
{
	t_redir	*newr;
	int		quoted;

	if (!cmd || !t || !is_redir(t->type) || !t->next || t->next->type != WORD)
		return (1);
	quoted = (t->next->qmask != 0);
	newr = redir_new(t->type, t->next->value, quoted);
	if (!newr)
		return (1);
	redir_add_back(&cmd->redirs, newr);
	return (0);
}
