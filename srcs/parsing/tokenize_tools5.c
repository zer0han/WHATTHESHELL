/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:46:49 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/13 15:27:39 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//call with assign_nosep_token(&input[i], &i);
char	*assign_nosep_token(char *input, int *i)
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
