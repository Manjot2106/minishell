/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:07:01 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 14:02:47 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	cmd_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	index;
	int	j;

	i = 1;
	while (cmd->argv[i])
	{
		index = env_index(shell->envp, cmd->argv[i]);
		if (index != -1)
		{
			free(shell->envp[index]);
			j = index;
			while (shell->envp[j])
			{
				shell->envp[j] = shell->envp[j + 1];
				j++;
			}
		}
		i++;
	}
	return (0);
}
