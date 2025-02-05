/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:26:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/05 19:58:37 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quote_after_dollar(char *input, char *ninput, int *i, int *j)
{
	if (input[*i + 1] == '\'')
		(*i)++;
	else if (input[*i + 1] == '\"')
		ninput[(*j)++] = input[(*i)++];
}
