/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:26 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/09 18:48:48 by rdalal           ###   ########.fr       */
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

/*void	free_tokens(t_token *tokens)
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
}*/

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->right;
		free(temp -> input);
		free(temp);
	}
}

void	free_exec(t_exec *exec_list)
{
	t_exec	*temp;
	
	while (exec_list)
	{
		temp = exec_list;
		exec_list = exec_list->next;
		if (temp->args)
			free_array(temp->args);
		if (temp->redir)
			free(temp->redir);
		free (temp);
	}
}

void	free_all(t_token *tokens, t_exec *exec_list)
{
	if (tokens)
		free_tokens(tokens);
	if (exec_list)
		free_exec(exec_list);
}
