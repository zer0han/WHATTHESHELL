/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:50:56 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/05 19:50:29 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char s)
{
	if (s == ' ' || (s >= '\t' && s <= '\r'))
		return (1);
	return (0);
}

int	ft_search_unquote(char *line, char quote)
{
	int	i;

	i = 1;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		return (i);
	return (INT_MIN);
}

int	ft_count_tokens(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i] && is_space(line[i]))
		i++;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			i += ft_search_unquote(&line[i], line[i]);
			count++;
		}
		else if (is_space(line[i]) == 0 && is_space(line[i + 1]) == 1)
			count++;
		else if (ft_isalnum(line[i]) && !ft_isalnum(line[i + 1]))
			count++;
		if (i < 0)
			return (-1);
		i++;
	}
	return (count);
}

char	*ft_strncdup(char const *src)
{
	char	*dest;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (is_space(src[len]) == 0)
		len++;
	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
		return (NULL);
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	alloc_fail(char **result, int iword)
{
	int	i;

	i = 0;
	while (i < iword)
	{
		if (result[i])
			free (result[i]);
		i++;
	}
	free (result);
}
