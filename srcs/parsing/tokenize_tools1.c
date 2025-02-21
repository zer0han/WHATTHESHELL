/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:50:56 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/21 19:05:29 by gmechaly         ###   ########.fr       */
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
	return (printf("closing quote missing\n"), INT_MIN);
}

int	ft_count_tokens(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			i++;
		else if (is_nosep_token(&line[i], &i))
			count++;
		else if (line[i] == '\'' || line[i] == '\"')
		{
			i += ft_search_unquote(&line[i], line[i]) + 1;
			count++;
		}
		else if (!is_space(line[i]) && line[i + 1] != '\"' \
				&& line[i + 1] != '\'')
		{
			while (line[i] && is_space(line[i]) == 0 \
			&& line[i] != '<' && line[i] != '>' && line[i] != '|')
				i++;
			count++;
		}
		if (i < 0)
			return (-1);
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
	while (src[len] && is_space(src[len]) == 0 \
	&& src[len] != '<' && src[len] != '>' && src[len] != '|')
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
