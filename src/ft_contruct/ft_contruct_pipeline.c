/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_contruct_pipeline.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:11:34 by msingh            #+#    #+#             */
/*   Updated: 2026/03/18 14:39:28 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	cmd_free_fields(t_command *cmd)
{
	free_redirs(cmd->redirs);
	cmd->redirs = NULL;
}

static t_token	*fill_command(t_command *cmd, t_token *t)
{
	t_argvb	b;

	ft_bzero(cmd, sizeof(*cmd));
	if (argvb_init(&b) < 0)
		return (NULL);
	while (t && t->type != PIPE)
	{
		if (is_redir(t->type))
		{
			if (redir_push_from_token(cmd, t) != 0)
				return (argvb_free(&b), cmd_free_fields(cmd), NULL);
			t = t->next->next;
		}
		else if (t->type == WORD && t->value[0] == '\0' && t->qmask == 0)
			t = t->next;
		else if (t->type == WORD && argvb_push(&b, t->value) == 0)
			t = t->next;
		else
			return (argvb_free(&b), cmd_free_fields(cmd), NULL);
	}
	cmd->argv = b.v;
	if (t && t->type == PIPE)
		return (t->next);
	return (t);
}

t_pipeline	*ft_construct_pipeline(t_token *token)
{
	t_pipeline	*p;
	int			i;
	t_token		*t;

	p = ft_calloc(1, sizeof(*p));
	if (!p)
		return (NULL);
	p->nb_commands = count_cmds(token);
	p->commands = ft_calloc(p->nb_commands, sizeof(t_command));
	if (!p->commands)
		return (free(p), NULL);
	i = 0;
	t = token;
	while (i < p->nb_commands)
	{
		t = fill_command(&p->commands[i], t);
		if (!t && i + 1 < p->nb_commands)
			return (ft_free(p), NULL);
		i++;
	}
	return (p);
}
