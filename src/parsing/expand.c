/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/28 11:24:49 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	dollar_dquote_intro(t_token *t, int i)
{
	if (!t->qstate || t->value[i] != '$' || !t->value[i + 1])
		return (0);
	if (t->qstate[i] == 'D' || t->qstate[i] == 'd')
		return (0);
	return (t->qstate[i + 1] == 'D' || t->qstate[i + 1] == 'd');
}

static int	process_dollar(t_token *t, t_bufpair *bp, int *i, char **envp)
{
	if (dollar_dquote_intro(t, *i))
		return ((*i)++, 1);
	if (t->value[*i] != '$' || (t->qstate
			&& (t->qstate[*i] == 'S' || t->qstate[*i] == 's')))
		return (0);
	if (t->qstate && (t->qstate[*i] == 'D' || t->qstate[*i] == 'd')
		&& t->value[*i + 1]
		&& t->qstate[*i + 1] != t->qstate[*i])
		return ((*i)++, bufpair_push(bp, '$', '0'));
	if (expand_bounded(t, bp, i, envp) < 0)
		return (-1);
	if (t->value[*i] == '$')
		return (1);
	return (0);
}

static int	expand_loop(t_token *t, t_bufpair *bp, int last_status, char **envp)
{
	int	i;
	int	rc;

	i = 0;
	while (t->value[i])
	{
		if (t->value[i] == '$' && (!t->qstate || (t->qstate[i] != 'S'
					&& t->qstate[i] != 's'))
			&& t->value[i + 1] == '?')
		{
			if (append_status(bp, last_status) < 0)
				return (-1);
			i += 2;
			continue ;
		}
		rc = process_dollar(t, bp, &i, envp);
		if (rc < 0)
			return (-1);
		if (rc > 0)
			continue ;
		if (bufpair_push(bp, t->value[i], '0') < 0)
			return (-1);
		i++;
	}
	return (i);
}

static int	expand_word_token(t_token *t, int last_status, char **envp)
{
	t_bufpair	bp;
	int			rc;

	if (!t->value)
		return (0);
	ft_bzero(&bp, sizeof(bp));
	if (bufpair_init(&bp) < 0)
		return (-1);
	rc = expand_loop(t, &bp, last_status, envp);
	if (rc < 0)
		return (free(bp.out), free(bp.st), -1);
	free(t->value);
	free(t->qstate);
	t->value = bp.out;
	t->qstate = bp.st;
	return (0);
}

int	ft_expand_tokens(t_token *tokens, int last_status, char **envp)
{
	t_token	*cur;
	t_token	*prev;

	cur = tokens;
	prev = NULL;
	while (cur)
	{
		if (cur->type == WORD && (!prev || prev->type != HEREDOC)
			&& expand_word_token(cur, last_status, envp) < 0)
			return (1);
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
