/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tools4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:50 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/05 19:59:29 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
				printf("path = %s\n", path);
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
	if (tmp->right && (tmp->right->type == NULL || \
	(!ft_strncmp(tmp->type, "cmd", 3) && \
	!ft_strncmp(tmp->right->type, "cmd", 3))))
	{
		if (tmp->right->input[0] == '-')
			tmp->right->type = "option";
		else
			tmp->right->type = "arg";
	}
}
