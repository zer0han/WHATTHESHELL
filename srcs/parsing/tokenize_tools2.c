/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:40:18 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/18 19:18:35 by gmechaly         ###   ########.fr       */
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

	len = 0;
	i = 0;
	// printf("%s\n", src);
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

void	*ft_split_for_tokens_2(char *line, char **result, int *i, int *iword)
{
	if (line[*i] == '\"' || line[*i] == '\'')
	{
		result[*iword] = ft_strnqdup(&line[*i], line[*i]);
		if (result[*iword] == NULL)
			return (alloc_fail(result, iword), NULL);
		*i += ft_strlen(result[*iword]) + 2;
		if (line[*i] == '\"' || line[*i] == '\'')
			(*i)++;
		(*iword)++;
	}
	else if (!is_space(line[*i]) && line[*i + 1] != '\"' \
			&& line[*i + 1] != '\'')
	{
		while (line[*i] && is_space(line[*i]))
			(*i)++;
		result[*iword] = ft_strncdup(&line[*i]);
		if (result[*iword] == NULL)
			return (alloc_fail(result, iword), NULL);
		*i += ft_strlen(result[*iword]);
		(*iword)++;
	}
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
	// printf("allocating for %d token(s)\n", nb_token + 1);
	result = (char **)malloc(sizeof(char *) * (nb_token + 1));
	if (result == NULL)
		return (NULL);
	while (line[i])
	{
		result[iword] = assign_nosep_token(&line[i], &i);
		if (result[iword] != NULL)
			iword++;
		if (ft_split_for_tokens_2(line, result, &i, &iword) == NULL)
			return (NULL);
		else
			i++;
	}
	result[iword] = NULL;
	return (result);
}
