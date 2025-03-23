/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_for_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:05:39 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/23 16:19:54 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_count_tokens_tool(char *line, int *i, int *count)
{
	while (*i >= 0 && line[*i] && is_space(line[*i]) == 0 \
	&& line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		if (is_quote(line[*i]))
			*i += ft_search_unquote(&line[*i], line[*i]) + 1;
		else
			(*i)++;
	}
	(*count)++;
}

static char	*assign_nosep_token(char *input, int *i)
{
	(*i)++;
	if (input[0] == '|')
		return (ft_strdup("|"));
	else if (input[0] == '<' && input[1] != input[0])
		return (ft_strdup("<"));
	else if (input[0] == '>' && input[1] != input[0])
		return (ft_strdup(">"));
	(*i)++;
	if (input[0] == '>' && input[1] == input[0])
		return (ft_strdup(">>"));
	else if (input[0] == '<' && input[1] == input[0])
		return (ft_strdup("<<"));
	else
	{
		(*i) -= 2;
		return (NULL);
	}
}

static int	ft_count_tokens(char *line)
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
			ft_count_tokens_tool(line, &i, &count);
		if (i < 0)
			return (-1);
	}
	return (count);
}

static void	*ft_split_for_tokens_2(char *line, char **res, int *i, int *iword)
{
	if (line[*i] == '\"' || line[*i] == '\'')
	{
		res[*iword] = ft_strnqdup(&line[*i], line[*i]);
		if (res[*iword] == NULL)
			return (alloc_fail(res, *iword), NULL);
		*i += ft_strlen(res[*iword]) + 2;
		if (line[*i] == '\"' || line[*i] == '\'')
			(*i)++;
		(*iword)++;
	}
	else if (!is_space(line[*i]) && line[*i + 1] != '\"' \
			&& line[*i + 1] != '\'')
	{
		while (line[*i] && is_space(line[*i]))
			(*i)++;
		res[*iword] = ft_strncdup(&line[*i]);
		if (res[*iword] == NULL)
			return (alloc_fail(res, *iword), NULL);
		*i += ft_strlen(res[*iword]);
		(*iword)++;
	}
	else
		(*i)++;
	return (*res);
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
		result[iword] = assign_nosep_token(&line[i], &i);
		if (result[iword] != NULL)
			iword++;
		if (ft_split_for_tokens_2(line, result, &i, &iword) == NULL)
			return (NULL);
	}
	result[iword] = NULL;
	return (result);
}
