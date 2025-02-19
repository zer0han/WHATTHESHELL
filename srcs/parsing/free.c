/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:26 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/19 12:59:07 by rdalal           ###   ########.fr       */
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
			break ;
		}
		free(tokens->left);
	}
}

void	free_exec(t_exec *exec_list)
{
	t_exec	*temp;
	
	while (exec_list)
	{
		temp = exec_list->next;
		free(exec_list->cmd);
		if (exec_list->args)
			free(exec_list->args);
		if (exec_list->redir)
			free(exec_list->redir);
		free (exec_list);
		exec_list = temp;
	}
}