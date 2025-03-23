/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:27:39 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/22 20:38:17 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_redir	*create_redir_node(t_redir_type type, char *target)
// {
// 	t_redir	*node;

// 	node = ft_calloc(1, sizeof(t_redir));
// 	if (node == NULL)
// 		return (NULL);
// 	node->type = type;;
// 	if (type == HEREDOC)
// 		node->delimiter = ft_strdup(target);
// 	else
// 		node->file = ft_strdup(target);
// 	return (node);
// }

// t_redir	*init_redir(t_token	**cmd_token)
// {
// 	t_token	*node;
// 	t_redir	*redir_list;
// 	t_redir	*new_redir;

// 	node = *cmd_token;
// 	redir_list = NULL;
// 	while (node)
// 	{
// 		if (is_redirection(node->input))
// 		{
// 			new_redir = malloc(sizeof(t_redir));
// 			if (!ft_strcmp(node->input, ">"))
// 				new_redir->type = REDIR_OUT;
// 			else if (!ft_strcmp(node->input, ">>"))
// 				new_redir->type = REDIR_APPEND;
// 			else if (!ft_strcmp(node->input, "<"))
// 				new_redir->type = REDIR_IN;
// 			else if (!ft_strcmp(node->input, "<<"))
// 				new_redir->type = HEREDOC;
// 			if (node->right)
// 				new_redir->file = ft_strdup(node->right->input);
// 			new_redir->next = redir_list;
// 			redir_list = new_redir;
// 		}
// 		node = node->right;
// 	}
// 	return (redir_list);
// }


void	cleanup_redirection(t_redir *redir)
{
	t_redir	*tmp;

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
			free(tmp->delimiter);
			tmp->delimiter = NULL;
		}
		free(tmp);
	}
}

static int	parse_redir_node(t_token **node, t_redir **tail)
{
	t_redir	*new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (0);
	if (!(*node)->right || !(*node)->right->input)
	{
		free(new);
		ft_putstr_fd("Syntax error\n", 2);
		return (0);
	}
	if (ft_strcmp((*node)->input, ">") == 0)
		new->type = REDIR_OUT;
	else if (ft_strcmp((*node)->input, ">>") == 0)
		new->type = REDIR_APPEND;
	else if (ft_strcmp((*node)->input, "<") == 0)
		new->type = REDIR_IN;
	else if (ft_strcmp((*node)->input, "<<") == 0)
		new->type = HEREDOC;
	if (new->type == HEREDOC)
		new->delimiter = ft_strdup((*node)->right->input);
	else
		new->file = ft_strdup((*node)->right->input);

	if (!(*node)->right || !(*node)->right->input)
	{
		free(new->file);
		free(new->delimiter);
		free(new);
		ft_putstr_fd("WHATTHESHELL: syntax error\n", 2);
		return (0);
	}
	*tail = new;
	*node = (*node)->right->right;
	return (1);
}

t_redir	*init_redir(t_token **cmd_token)
{
	t_token	*node;
	t_redir	*redir_list;
	t_redir	**tail;

	node = *cmd_token;
	redir_list = NULL;
	tail = &redir_list;
	while (node->left && (ft_strcmp(node->left->type, "pipe") != 0))
		node = node->left;
	fprintf(stderr, "DEBUG: [init_redir] starting at token %s\n", node->input);
	while (node && (ft_strcmp(node->type, "pipe") != 0))
	{
		fprintf(stderr, "DEBUG: [init_redir] processing token %s\n", node->input);
		if (ft_strcmp(node->type, "redirection") == 0 
			|| ft_strcmp(node->type, "append mode") == 0 
			|| ft_strcmp(node->type, "heredoc") == 0)
		{
			fprintf(stderr, "DEBUG: [init_redir] redirection initialized and processing token here %s\n", node->input);
			if (!parse_redir_node(&node, tail))
				return (cleanup_redirection(redir_list), NULL);
		}
		else
			node = node->right;
	}
	return (redir_list);
}

