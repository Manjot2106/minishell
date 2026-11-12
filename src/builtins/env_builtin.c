/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 16:06:42 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/25 14:03:20 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	cmd_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell->envp)
		return (1);
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
		{
			ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}
