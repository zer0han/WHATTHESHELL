/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:16:24 by rdalal            #+#    #+#             */
/*   Updated: 2024/09/25 19:27:34 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static size_t	ft_start_trim(const char *s1, const char *set)
{
	size_t	start;

	start = 0;
	while (s1[start] && ft_set(s1[start], set))
		start++;
	return (start);
}

static size_t	ft_end_trim(const char *s1, const char *set, size_t start)
{
	size_t	end;

	end = ft_strlen(s1);
	while (end > start && ft_set(s1[end - 1], set))
		end--;
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	i;
	size_t	end;
	size_t	start;
	size_t	len;

	i = 0;
	start = ft_start_trim(s1, set);
	end = ft_end_trim(s1, set, start);
	len = end - start;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (i < len)
	{
		result[i] = s1[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
