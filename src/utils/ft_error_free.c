/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:06:19 by msingh            #+#    #+#             */
/*   Updated: 2026/03/12 13:54:52 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		if (r->fd != -1 && r->type == HEREDOC)
			close(r->fd);
		free(r->target);
		free(r);
		r = next;
	}
}

void	ft_free(t_pipeline *p)
{
	int	i;

	if (!p)
		return ;
	i = 0;
	while (i < p->nb_commands)
	{
		free_tab(p->commands[i].argv);
		free_redirs(p->commands[i].redirs);
		i++;
	}
	free(p->commands);
	free(p);
}

void	ft_free_tokens(t_token *toks)
{
	t_token	*next;

	while (toks)
	{
		next = toks->next;
		free(toks->value);
		free(toks->qstate);
		free(toks);
		toks = next;
	}
}

int	error_close(char *msg, int fd)
{
	perror(msg);
	if (fd >= 0)
		close(fd);
	return (1);
}
