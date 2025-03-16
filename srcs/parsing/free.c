/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:26 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/13 17:47:00 by gmechaly         ###   ########.fr       */
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

void	free_array(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

/*void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->right;
		if (tokens->input)
		{
			free(tokens->input);
			tokens->input = NULL;
		}
		free(tokens);
		tokens = temp;
	}
}*/

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->right;
		if (temp->input)
		{
			free(temp->input);
			temp->input = NULL;
		}
		if (temp)
			free(temp);
		temp = NULL;
	}
	tokens = NULL;
}

void	free_exec(t_exec *exec_list)
{
	t_exec	*temp;
	int		i;
	
	while (exec_list)
	{
		temp = exec_list;
		exec_list = exec_list->next;
		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
				free(temp->args[i++]);
			free(temp->args);
		}
		if (temp->redir)
			free(temp->redir);
		free (temp);
	}
}

void	free_all(t_token *tokens, t_exec *exec_list)
{
	if (tokens)
	{
		free_tokens(tokens);
		tokens = NULL;
	}
	if (exec_list)
	{
		free_exec(exec_list);
		exec_list = NULL;
	}
}

void	free_envp(t_envp *dup)
{
	t_envp	*temp;

	temp = dup;
	while (dup)
	{
		free(dup->str);
		temp = temp->next;
		free(dup);
		dup = temp;
	}
}
