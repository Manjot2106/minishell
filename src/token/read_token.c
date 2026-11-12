/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 14:02:10 by msingh            #+#    #+#             */
/*   Updated: 2026/03/28 11:24:49 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_word_char(char *line, int *ctx, char *quote,
		t_bufpair *bp)
{
	char	qstate;

	if (handle_quote(line, &ctx[0], quote, &ctx[1]))
	{
		if (*quote == '"' && ctx[2] == 'D')
			ctx[2] = 'd';
		else if (*quote == '"' && ctx[2] != 'D')
			ctx[2] = 'D';
		else if (*quote == '\'' && ctx[3] == 'S')
			ctx[3] = 's';
		else if (*quote == '\'' && ctx[3] != 'S')
			ctx[3] = 'S';
		return (0);
	}
	qstate = '0';
	if (*quote == '"')
		qstate = (char)ctx[2];
	else if (*quote == '\'')
		qstate = (char)ctx[3];
	if (bufpair_push(bp, line[ctx[0]], qstate) < 0)
		return (-1);
	ctx[0]++;
	return (0);
}

static int	read_word_loop(char *line, int *i, t_bufpair *bp, int *qmask)
{
	char	quote;
	int		ctx[4];

	quote = 0;
	ctx[0] = *i;
	ctx[1] = 0;
	ctx[2] = 'D';
	ctx[3] = 'S';
	while (line[ctx[0]] && (quote || (!is_space(line[ctx[0]])
				&& !is_meta(line[ctx[0]]))))
	{
		if (process_word_char(line, ctx, &quote, bp) < 0)
			return (-1);
	}
	*i = ctx[0];
	*qmask = ctx[1];
	if (quote)
		return (-2);
	return (0);
}

int	ft_read_word(char *line, int *i, t_word *w, t_parse_err *err)
{
	t_bufpair	bp;
	int			rc;

	ft_bzero(&bp, sizeof(bp));
	if (bufpair_init(&bp) < 0)
		return (*err = P_ERR_MALLOC, -1);
	rc = read_word_loop(line, i, &bp, &w->qmask);
	if (rc == -2)
		return (free(bp.out), free(bp.st), *err = P_ERR_QUOTE, -1);
	if (rc < 0)
		return (free(bp.out), free(bp.st), *err = P_ERR_MALLOC, -1);
	w->str = bp.out;
	w->qstate = bp.st;
	*err = P_OK;
	return (0);
}

static int	push_meta_token(t_token **toks, t_token_type type, const char *s,
		t_parse_err *err)
{
	char	*dup;
	t_token	*new;

	dup = ft_strdup(s);
	if (!dup)
		return (*err = P_ERR_MALLOC, -1);
	new = token_new(type, dup, 0, NULL);
	if (!new)
		return (free(dup), *err = P_ERR_MALLOC, -1);
	token_add_back(toks, new);
	return (0);
}

int	ft_push(t_token **toks, char *line, int i, t_parse_err *err)
{
	if (line[i] == '>' && line[i + 1] == '>')
		return (push_meta_token(toks, REDIR_APPEND, ">>", err), i + 2);
	if (line[i] == '<' && line[i + 1] == '<')
		return (push_meta_token(toks, HEREDOC, "<<", err), i + 2);
	if (line[i] == '|')
		return (push_meta_token(toks, PIPE, "|", err), i + 1);
	if (line[i] == '<')
		return (push_meta_token(toks, REDIR_IN, "<", err), i + 1);
	if (line[i] == '>')
		return (push_meta_token(toks, REDIR_OUT, ">", err), i + 1);
	return (i);
}
