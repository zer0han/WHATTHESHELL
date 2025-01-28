/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:05:00 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/28 14:53:43 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*is_special_str(t_token **tokens, char *str)
{
	t_token	*node;

	node = *tokens;
	while (node)
	{
		if (!ft_strncmp(node->input, str, ft_strlen(node->input)))
			return (node);
		node = node->right;
	}
	return (NULL);
}

void	*assign_token_type(t_token **tokens)
{
	t_token	*node;

	node = is_special_str(tokens, "<<");
	if (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "delimiter";
			node->type = "heredoc";
			node->left->type = "cmd";
		}
		else
			return (NULL);
	}
	node = is_special_str(tokens, ">>");
	if (node != NULL)
	{
		if (node->left && node->right)
		{
			node->type = "append mode";
			node->right->type = "file";
		}
		else
			return (NULL);
	}
	return (assign_token_type2(tokens));	
}

void	*assign_token_type2(t_token **tokens)
{
	t_token	*node;

	node = is_special_str(tokens, "|");
	if (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "cmd";
			node->type = "pipe";
		}
		else
			return (NULL);
	}
	node = is_special_str(tokens, "<");
	if (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "cmd";
			node->type = "redirection";
			node->left->type = "file";
		}
		else
			return (NULL);
	}
	return (assign_token_type3(tokens));
}

void	*assign_token_type3(t_token **tokens)
{
	t_token	*node;

	node = is_special_str(tokens, ">");
	if (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "file";
			node->type = "redirection";
		}
		else
			return (NULL);
	}
	return (*tokens);
}
