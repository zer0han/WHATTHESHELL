/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:40:18 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/16 17:27:09 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*ft_lastnode(t_token *tokens)
{
	if (tokens == NULL)
		return (NULL);
	while (tokens->right != NULL && tokens != NULL)
		tokens = tokens->right;
	return (tokens);
}

char	*ft_strnqdup(char *src, char quote)
{
	char	*dst;
	int		len;
	int		i;

	len = 1;
	i = 0;
	while (src[len] != quote)
		len++;
	dst = (char *)malloc(sizeof(char) * (len + 2));
	if (dst == NULL)
		return (NULL);
	while (i < len + 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	**ft_split_for_tokens(char *line)
{
	char	**result;
	int		i;
	int		iword;
	int		nb_token;

	i = 0;
	iword = 0;
	nb_token = ft_count_tokens(line);
	if (nb_token < 0)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (nb_token + 1));
	if (result == NULL)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			result[iword] = ft_strnqdup(&line[i], line[i]);
			if (result[iword] == NULL)
				return (alloc_fail(result, iword), NULL);
			i += ft_strlen(result[iword]);
			if (line[i] == '\"' || line[i] == '\'')
				i++;
			iword++;
		}
		else if (!is_space(line[i]) && line[i + 1] != '\"' \
				&& line[i + 1] != '\'')
		{
			while (line[i] && is_space(line[i]))
				i++;
			result[iword] = ft_strncdup(&line[i]);
			if (result[iword] == NULL)
				return (alloc_fail(result, iword), NULL);
			i += ft_strlen(result[iword]);
			iword++;
		}
		else
			i++;
	}
	result[iword] = NULL;
	return (result);
}
