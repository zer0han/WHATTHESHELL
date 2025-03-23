/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:21:49 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/23 20:16:38 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**setup_cmd(t_token *node)
{
	char	**cmd;
	int		i;

	i = 0;
	while (node && \
	(!ft_strcmp(node->type, "arg") || !ft_strcmp(node->type, "option")))
	{
		i++;
		node = node->right;
	}
	cmd = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node && \
	(!ft_strcmp(node->type, "arg") || !ft_strcmp(node->type, "option")))
	{
		cmd[i] = ft_strdup(node->input);
		node = node->right;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

int	multi_line_part()
{
	
}

int	heredoc(t_token **tokens)
{
	t_token	*node;
	char	*delim;
	char	*buf;
	char	**cmd;

	signal(SIGINT, SIG_DFL);
	node = *tokens;
	cmd = setup_cmd(node);
	while (node && ft_strcmp(node->type, "delimiter"))
		node = node->right;
	delim = ft_strdup(node->input);
}
