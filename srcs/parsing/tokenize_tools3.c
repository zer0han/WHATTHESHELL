/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:05:00 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/16 15:07:23 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

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
		if (node->right)
		{
			node->right->type = "delimiter";
			node->type = "heredoc";
		}
		else
			return (printf("parse error near << token"), NULL);
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
			return (printf("parse error near >> token"), NULL);
	}
	return (assign_token_type2(tokens, node));
}

void	*assign_token_type2(t_token **tokens, t_token *node)
{
	node = is_special_str(tokens, "<");
	while (node != NULL)
	{
		if (node->right)
		{
			node->type = "redirection";
			node->right->type = "file";
			node = is_special_str(&node->right, "<");
		}
		else
			return (printf("parse error near < token"), NULL);
	}
	node = is_special_str(tokens, ">");
	while (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "file";
			node->type = "redirection";
			node = is_special_str(&node->right, ">");
		}
		else
			return (printf("parse error near > token"), NULL);
	}
	return (assign_token_type3(tokens, node));
}

void	*assign_token_type3(t_token **tokens, t_token *node)
{
	node = is_special_str(tokens, "|");
	while (node != NULL)
	{
		if (node->right && node->left)
		{
			node->right->type = "cmd";
			node->type = "pipe";
			node = is_special_str(&node->right, "|");
		}
		else
			return (printf("parse error near | token"), NULL);
	}
	return (*tokens);
}
