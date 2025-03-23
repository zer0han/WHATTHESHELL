/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:05:00 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/23 20:35:53 by rdalal           ###   ########.fr       */
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
			return (printf("parse error near | token"), NULL);
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
	return (assign_token_pipe(tokens, node));
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
		if (node->right) //if (node->left && node->right)
		{
			node->type = "append mode";
			node->right->type = "file";
		}
		else
			return (printf("parse error near >> token"), NULL);
	}
	return (assign_token_redir(tokens, node));
}

void	assign_missing_cmds(t_token **tokens)
{
	t_token	*node;
	char	*path;
	int		is_pipe;
	int		is_cmd;

	node = *tokens;
	is_pipe = 0;
	is_cmd = 0;
	while (node)
	{
		if (node->type && !ft_strncmp(node->type, "pipe", 4))
			is_pipe++;
		else if (node->type == NULL && is_cmd <= is_pipe)
		{
			path = get_path(node->input);
			if (path != NULL || fd_is_builtin(node))
				node->type = "cmd";
			free(path);
		}
		if (node->type && !ft_strncmp(node->type, "cmd", 4))
			is_cmd++;
		node = node->right;
	}
}

void	assign_options_and_args(t_token **tokens)
{
	t_token	*node;

	node = *tokens;
	while (node)
	{
		if (node->type && \
		!ft_strncmp(node->type, "cmd", ft_strlen(node->type)))
			parse_after_cmds(&node);
		node = node->right;
	}
}
