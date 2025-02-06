/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:26 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/29 17:48:24 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_string_tab(char **str_tab)
{
	int	i;

	i = 0;
	while (str_tab[i])
	{
		free(str_tab[i]);
		i++;
	}
	free(str_tab);
}

void	free_tokens(t_token *tokens)
{
	while (tokens)
	{
		free(tokens->input);
		if (tokens->right)
			tokens = tokens->right;
		else
		{
			free(tokens);
			break;
		}
		free(tokens->left);
	}
}
