/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:47:53 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/29 15:35:04 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all_before_exit(t_shell *shell, t_pipeline *p, int status)
{
	if (p)
		ft_free(p);
	if (shell && shell->envp)
		free_tab(shell->envp);
	rl_clear_history();
	exit(status);
}

void	print_cmd_not_found(char *cmd)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("minishell: ", cmd);
	if (!tmp)
		return ;
	msg = ft_strjoin(tmp, ": command not found\n");
	free(tmp);
	if (!msg)
		return ;
	write(2, msg, ft_strlen(msg));
	free(msg);
}
