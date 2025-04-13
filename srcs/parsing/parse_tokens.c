/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:50 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/13 17:14:30 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	*parse_tokens(t_token **tokens, t_envp *env)
{
	t_token	*node;
	char	*path;

	node = *tokens;
	while (node)
	{
		if (node->type && !ft_strncmp(node->type, "cmd", 3))
		{
			path = get_path(node->input, env);
			if (path == NULL && !fd_is_builtin(node))
				return (printf(PROMPT "%s : invalid cmd\n", node->input), \
					g_exit_status = 127, NULL);
			free(path);
		}
		else if (node->type && !(ft_strncmp(node->type, "file", 4)))
		{
			if (node->left && node->left->input && \
			(!ft_strncmp(node->left->input, "<", ft_strlen(node->left->input))) \
			&& access(node->input, F_OK | R_OK))
				return (printf(PROMPT "%s : no such file or directory\n", \
						node->input), g_exit_status = 1, NULL);
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
		return (printf(PROMPT "%s : invalid cmd\n", node->input), \
			g_exit_status = 127, NULL);
	while (node)
	{
		if (node->type == NULL)
			return (NULL);
		node = node->right;
	}
	return (*tokens);
}
