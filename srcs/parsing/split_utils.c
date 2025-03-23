/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:38:42 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/21 19:55:09 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_search_unquote(const char *line, char quote)
{
	int	i;

	i = 1;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		return (i);
	return (printf("closing quote missing\n"), INT_MIN);
}

int	is_nosep_token(char *input, int *i)
{
	(*i)++;
	if (input[0] == '|')
		return (1);
	else if ((input[0] == '<' || input[0] == '>') && input[1] != input[0])
		return (1);
	(*i)++;
	if ((input[0] == '<' || input[0] == '>') && input[1] == input[0])
		return (1);
	else
	{
		(*i) -= 2;
		return (0);
	}
}
