/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:26:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/21 19:09:25 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quote_after_dollar(char *input, char *ninput, int *i, int *j)
{
	if (input[*i + 1] == '\'')
		(*i)++;
	else if (input[*i + 1] == '\"')
	{
		ninput[*j] = input[*i];
		*i += 2;
		(*j)++;
		while (input[*i] && input[*i] != '<' && input[*i] != '>' && \
		input[*i] != '|' && input[*i] != '\"')
		{
			ninput[*j] = input[*i];
			(*i)++;
			(*j)++;
		}
		ninput[*j] = '\0';
		if (input[*i] == '\"')
			(*i)++;
	}
}

void	copy_quote(char *input, char *ninput, int *i, int *j)
{
	if (input[*i] == '\'')
		copy_squote(input, ninput, i, j);
	else if (input[*i] == '\"')
		copy_dquote(input, ninput, i, j);
}

void	copy_dquote(char *input, char *ninput, int *i, int *j)
{
	if (input[*i] == '\"')
		ninput[(*j)++] = input[(*i)++];
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$' && input[(*i) + 1])
			replace_var_by_value(input, ninput, i, j);
		else
			ninput[(*j)++] = input[(*i)++];
	}
	if (input[*i] == '\"')
		ninput[(*j)++] = input[(*i)++];
}

void	copy_squote(char *input, char *ninput, int *i, int *j)
{
	if (input[*i] == '\'')
		ninput[(*j)++] = input[(*i)++];
	while (input[*i] != '\'' && input[*i])
		ninput[(*j)++] = input[(*i)++];
	if (input[*i] == '\'')
		ninput[(*j)++] = input[(*i)++];
}
