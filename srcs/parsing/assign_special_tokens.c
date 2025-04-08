/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_special_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 00:01:43 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:44:10 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*assign_token_pipe(t_token **tokens, t_token *node)
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
			return (printf("parse error near | token\n"), NULL);
	}
	return (*tokens);
}

static void	*assign_token_redir(t_token **tokens, t_token *node)
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
			return (printf("parse error near < token\n"), NULL);
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
			return (printf("parse error near > token\n"), NULL);
	}
	return (assign_token_pipe(tokens, node));
}

static void	*assign_token_append(t_token **tokens, t_token *node)
{
	node = is_special_str(tokens, ">>");
	while (node != NULL)
	{
		if (node->right)
		{
			node->type = "append mode";
			node->right->type = "file";
			node = is_special_str(&node->right, ">>");
		}
		else
			return (printf("parse error near >> token\n"), NULL);
	}
	return (assign_token_redir(tokens, node));
}

void	*assign_token_type(t_token **tokens)
{
	t_token	*node;

	node = is_special_str(tokens, "<<");
	while (node != NULL)
	{
		if (node->right)
		{
			node->right->type = "delimiter";
			node->type = "heredoc";
			node = is_special_str(&node->right, "<<");
		}
		else
			return (printf("parse error near << token\n"), NULL);
	}
	return (assign_token_append(tokens, node));
}
