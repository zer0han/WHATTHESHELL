/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:27:39 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/09 20:36:51 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_redirection(t_redir *redir)
{
	t_redir	*tmp;

	while (redir && redir->prev)
		redir = redir->prev;
	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->file)
		{
			free(tmp->file);
			tmp->file = NULL;
		}
		if (tmp->delimiter)
		{
			free_array(tmp->delimiter);
			tmp->delimiter = NULL;
		}
		free(tmp);
	}
}

static int	count_delims(t_token **tokens)
{
	int		i;
	t_token	*node;

	node = *tokens;
	i = 0;
	while (node && (!ft_strcmp(node->type, "heredoc") \
	|| !ft_strcmp(node->type, "delimiter")))
	{
		if (!ft_strcmp(node->type, "delimiter"))
			i++;
		node = node->right;
	}
	return (i);
}

void	*create_delim_array(t_redir *redir, t_token **tokens)
{
	int	i;
	int	count;

	i = 0;
	count = count_delims(tokens);
	redir->delimiter = malloc(sizeof(char *) * (count + 1));
	if (redir->delimiter == NULL)
		return (NULL);
	while (*tokens && (!ft_strcmp((*tokens)->type, "heredoc") \
	|| !ft_strcmp((*tokens)->type, "delimiter")))
	{
		if (!ft_strcmp((*tokens)->type, "delimiter"))
		{
			redir->delimiter[i] = ft_strdup((*tokens)->input);
			if (redir->delimiter[i] == NULL)
				return (NULL);
			i++;
		}
		tokens = &(*tokens)->right;
	}
	redir->delimiter[i] = NULL;
	return (redir);
}

t_redir	*ft_last_redir_node(t_redir **redir)
{
	t_redir	*node;

	if (!*redir)
		return (NULL);
	node = *redir;
	while (node && node->next)
		node = node->next;
	return (node);
}

t_redir	*init_redir(t_token **cmd_token)
{
	t_token	*node;
	t_redir	*redir_list;

	node = *cmd_token;
	redir_list = NULL;
	while (node->left && (ft_strcmp(node->left->type, "pipe") != 0))
		node = node->left;
	while (node && (ft_strcmp(node->type, "pipe") != 0))
	{
		if (!ft_strcmp(node->type, "redirection") || \
		!ft_strcmp(node->type, "append mode") || \
		!ft_strcmp(node->type, "heredoc"))
		{
			if (!parse_redir_node(&node, &redir_list))
				return (cleanup_redirection(redir_list), NULL);
			while (node && (!ft_strcmp(node->type, "heredoc") \
			|| !ft_strcmp(node->type, "delimiter")))
				node = node->right;
		}
		else
			node = node->right;
	}
	return (redir_list);
}
