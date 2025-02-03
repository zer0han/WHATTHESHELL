/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:05:00 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/30 16:54:19 by gmechaly         ###   ########.fr       */
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
	return (assign_token_type2(tokens, node));
}

void	*assign_token_type2(t_token **tokens, t_token *node)
{
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
	return (assign_token_type3(tokens, node));
}

void	*assign_token_type3(t_token **tokens, t_token *node)
{
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
	node = *tokens;
	while (node)
	{
		if (node->input[0] == '$')
			node->type = "variable to expand";
		node = node->right;
	}
	return (*tokens);
}
