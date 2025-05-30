/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_libft.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:34:08 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/08 22:02:28 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_strncdup_len(const char *src)
{
	int	len;

	len = 0;
	while (len >= 0 && src[len] && is_space(src[len]) == 0 \
	&& src[len] != '<' && src[len] != '>' && src[len] != '|')
	{
		if (is_quote(src[len]))
			len += ft_search_unquote(&src[len], src[len]) + 1;
		else
			len++;
	}
	return (len);
}

char	*ft_strncdup(char const *src, int *index)
{
	char	*dest;
	int		len;
	int		i;
	int		not_copied;

	len = 0;
	i = 0;
	not_copied = 0;
	len = ft_strncdup_len(src);
	if (len >= 0)
		dest = (char *)malloc(sizeof(char) * (len + 1));
	if (len < 0 || dest == NULL)
		return (NULL);
	while (i < len)
	{
		if (!is_quote(src[i]))
			dest[i - not_copied] = src[i];
		else
			not_copied++;
		i++;
	}
	*index += i;
	dest[i - not_copied] = '\0';
	return (dest);
}

char	*ft_strnqdup(char *src, char quote)
{
	char	*dst;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (src[len] == quote)
		len++;
	while (src[len] && src[len] != quote)
		len++;
	dst = (char *)malloc(sizeof(char) * (len + 1));
	if (dst == NULL)
		return (NULL);
	while (src[i + 1] && src[i + 1] != quote)
	{
		dst[i] = src[i + 1];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_fstrjoin(char *s1, char *s2)
{
	char	*dst;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	dst = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dst == NULL)
		return (free(s1), NULL);
	while (s1 != NULL && s1[i])
		dst[j++] = s1[i++];
	i = 0;
	free (s1);
	while (s2 != NULL && s2[i])
		dst[j++] = s2[i++];
	dst[j] = '\0';
	return (dst);
}
