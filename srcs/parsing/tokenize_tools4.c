/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:50 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/21 19:07:29 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	assign_missing_cmds(t_token **tokens)
{
	t_token	*node;
	char	*path;

	node = *tokens;
	while (node)
	{
		if (node->type == NULL)
		{
			path = get_path(node->input);
			if (path != NULL || is_builtin(node->input))
			{
				node->type = "cmd";
				free(path);
			}
		}
		node = node->right;
	}
}

void	assign_options_and_args(t_token **tokens)
{
	t_token	*node;

	node = *tokens;
	while (node)
	{
		if (node->type && !ft_strncmp(node->type, "cmd", ft_strlen(node->type)))
			parse_after_cmds(&node);
		node = node->right;
	}
}

void	parse_after_cmds(t_token **node)
{
	t_token	*tmp;

	tmp = *node;
	while (tmp->right && (tmp->right->type == NULL || \
	(!ft_strncmp(tmp->type, "cmd", 3) && \
	!ft_strncmp(tmp->right->type, "cmd", 3))))
	{
		if (tmp->right->input[0] == '-')
			tmp->right->type = "option";
		else
			tmp->right->type = "arg";
		tmp = tmp->right;
	}
}

void	*parse_tokens(t_token **tokens)
{
	t_token	*node;
	char	*path;

	node = *tokens;
	while (node)
	{
		if (node->type && !ft_strncmp(node->type, "cmd", 3))
		{
			path = get_path(node->input);
			if (path == NULL && !is_builtin(node->input))
				return (free(path), printf("%s : invalid cmd\n", node->input), NULL);
			free(path);
		}
		else if (node->type && !(ft_strncmp(node->type, "file", 4)))
		{
			if (node->left && node->left->input && \
			(ft_strncmp(node->left->input, ">", ft_strlen(node->left->input))))
			{
				if (access(node->input, F_OK | R_OK))
					return (printf("%s : no such file or directory\n", node->input), NULL);
			}
		}
		node = node->right;
	}
	return (*tokens);
}

void	*is_null_token(t_token **tokens)
{
	t_token	*node;

	node = *tokens;
	if (node->type == NULL)
		return (printf("%s : invalid cmd\n", node->input), NULL);
	while (node)
	{
		if (node->type == NULL)
			return (NULL);
		node = node->right;
	}
	return (*tokens);
}
