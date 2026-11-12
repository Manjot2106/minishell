/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 12:16:47 by msingh            #+#    #+#             */
/*   Updated: 2026/03/18 14:39:25 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	print_parse_error(t_parse_err err)
{
	if (err == P_ERR_QUOTE)
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
	else if (err == P_ERR_MALLOC)
		ft_putendl_fd("minishell: memory allocation error", 2);
}

static void	set_parse_error(t_parse_err err, int *parse_status)
{
	if (!parse_status)
		return ;
	if (err == P_ERR_QUOTE)
		*parse_status = 2;
	else
		*parse_status = 1;
}

static t_pipeline	*expand_and_check(t_token *tokens, int last_status,
		char **envp, int *parse_status)
{
	t_pipeline	*pipeline;

	if (ft_expand_tokens(tokens, last_status, envp) != 0)
	{
		ft_putendl_fd("minishell: memory allocation error", 2);
		if (parse_status)
			*parse_status = 1;
		return (NULL);
	}
	if (ft_check_syntax(tokens) == -1)
	{
		if (parse_status)
			*parse_status = 2;
		return (NULL);
	}
	pipeline = ft_construct_pipeline(tokens);
	if (!pipeline && parse_status)
		*parse_status = 1;
	return (pipeline);
}

t_pipeline	*parse_line(char *line, int last_status, char **envp,
		int *parse_status)
{
	t_token		*tokens;
	t_pipeline	*pipeline;
	t_parse_err	err;

	if (parse_status)
		*parse_status = 0;
	tokens = ft_tokeniser(line, &err);
	if (!tokens)
	{
		if (err == P_OK)
			return (NULL);
		set_parse_error(err, parse_status);
		return (print_parse_error(err), NULL);
	}
	pipeline = expand_and_check(tokens, last_status, envp, parse_status);
	ft_free_tokens(tokens);
	return (pipeline);
}
