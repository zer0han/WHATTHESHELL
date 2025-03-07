/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:31:00 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/07 17:13:07 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_sep(char c, char sep)
{
	if (c == sep || c == '\0')
		return (1);
	return (0);
}

int	ft_word_count(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] && ft_sep(str[i], c))
		i++;
	while (str[i] != '\0')
	{
		if (!ft_sep(str[i], c) && ft_sep(str[i + 1], c))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_sepstrdup(char const *s, char c)
{
	char	*dest;
	int		i;
	int		j;

	j = 0;
	while (!ft_sep(s[j], c))
		j++;
	dest = (char *)malloc(j + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < j)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	free_split(char **split, int word_all)
{
	int	i;

	i = 0;
	while (i < word_all)
	{
		if (split[i])
		{
			free (split[i]);
			split[i] = NULL;
		}
		i++;
	}
	free (split);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		word_index;

	i = 0;
	word_index = 0;
	result = (char **)malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (s[i])
	{
		if (ft_sep(s[i], c))
			i++;
		else
		{
			result[word_index] = ft_sepstrdup(&s[i], c);
			if (!result[word_index])
				return (free_split(result, word_index), NULL);
			i += ft_strlen(result[word_index]);
			word_index++;
		}
	}
	result[word_index] = NULL;
	return (result);
}

