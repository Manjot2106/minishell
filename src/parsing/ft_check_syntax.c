/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:10:16 by msingh            #+#    #+#             */
/*   Updated: 2026/03/16 12:20:20 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	syntax_error(const char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(tok, 2);
	ft_putstr_fd("'\n", 2);
	return (-1);
}

static int	check_pipe(t_token *cur)
{
	if (cur->type != PIPE)
		return (0);
	if (!cur->next)
		return (syntax_error("newline"));
	if (cur->next->type == PIPE)
		return (syntax_error("|"));
	return (0);
}

static int	check_redir(t_token *cur)
{
	if (!is_redir(cur->type))
		return (0);
	if (!cur->next)
		return (syntax_error("newline"));
	if (cur->next->type != WORD)
		return (syntax_error(cur->next->value));
	return (0);
}

int	ft_check_syntax(t_token *token)
{
	t_token	*cur;

	if (!token)
		return (0);
	if (token->type == PIPE)
		return (syntax_error("|"));
	cur = token;
	while (cur)
	{
		if (check_pipe(cur) == -1)
			return (-1);
		if (check_redir(cur) == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}
