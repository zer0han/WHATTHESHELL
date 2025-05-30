/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:05:00 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 15:22:01 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_missing_cmds(t_token **tokens, t_envp *env)
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
			path = get_path(node->input, env);
			if (path != NULL || fd_is_builtin(node) || \
			!access(node->input, F_OK | X_OK))
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
