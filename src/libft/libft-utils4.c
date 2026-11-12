/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft-utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaddo <akaddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:07:15 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/12 13:10:25 by akaddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a'
			&& c <= 'z'))
		return (1);
	return (0);
}

static char	*ft_checkstr(char const *s, unsigned int start)
{
	size_t	l;
	char	*str;

	l = ft_strlen(s);
	if (start >= l)
	{
		str = malloc(1);
		if (!str)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	return (0);
}

static int	ft_checkint(char const *s, unsigned int start, size_t len)
{
	int	slen;

	slen = ft_strlen(s);
	if (len > slen - start)
		len = slen - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	j;

	str = ft_checkstr(s, start);
	if (str)
		return (str);
	len = ft_checkint(s, start, len);
	i = 0;
	j = 0;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		if (i == start)
		{
			while (j < len && s[i])
				str[j++] = s[i++];
			str[j] = '\0';
			return (str);
		}
		i++;
	}
	return (0);
}
