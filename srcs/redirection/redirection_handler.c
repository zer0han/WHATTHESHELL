/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:27:39 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/08 00:26:59 by gmechaly         ###   ########.fr       */
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

// static void	print_delim_array(char **delimiter)
// {
// 	int	i;

// 	i = 0;
// 	while (delimiter[i])
// 	{
// 		printf("delim[%d] = .%s.\n", i, delimiter[i]);
// 		i++;
// 	}
// }

static void	*create_delim_array(t_redir *redir, t_token **tokens)
{
	t_token	*head;
	int		i;

	i = 0;
	head = *tokens;
	while (*tokens && (!ft_strcmp((*tokens)->type, "heredoc") \
	|| !ft_strcmp((*tokens)->type, "delimiter")))
	{
		if (!ft_strcmp((*tokens)->type, "delimiter"))
			i++;
		*tokens = (*tokens)->right;
	}
	redir->delimiter = malloc(sizeof(char *) * (i + 1));
	if (redir->delimiter == NULL)
		return (NULL);
	tokens = &head;
	i = 0;
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

static t_redir	*ft_last_redir_node(t_redir **redir)
{
	t_redir	*node;

	if (!*redir)
		return (NULL);
	node = *redir;
	while (node && node->next)
		node = node->next;
	return (node);
}

static int	parse_redir_node(t_token **node, t_redir **tail)
{
	t_redir	*new;

	new = ft_calloc(1, sizeof(t_redir));
	*tail = ft_last_redir_node(tail);
	if (!new)
		return (0);
	if (!*tail)
	{
		new->prev = NULL;
		*tail = new;
	}
	else
	{
		new->prev = *tail;
		new->prev->next = new;
	}
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
	{
		if (!create_delim_array(new, node))
			return (0);
		// print_delim_array(new->delimiter);
	}
	else
		new->file = ft_strdup((*node)->right->input);
	// if (!(*node)->right || !(*node)->right->input)
	// {
	// 	free(new->file);
	// 	free_array(new->delimiter);
	// 	free(new);
	// 	ft_putstr_fd("PROMPT syntax error\n", 2);
	// 	return (0);
	// }
	// *tail = new;
	if (new->type != HEREDOC)
		*node = (*node)->right->right;
	new->next = NULL;
	return (1);
}

static void	print_redir_list(t_redir **redir)
{
	t_redir	*node;
	int		i;

	i = 0;
	node = *redir;
	while (node)
	{
		printf("redir[%d] : file = %s, type = %d\n", i, node->file, (int)node->type);
		node = node->next;
		i++;
	}
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
	while (node && (ft_strcmp(node->type, "pipe") != 0))
	{
		if (ft_strcmp(node->type, "redirection") == 0 
			|| ft_strcmp(node->type, "append mode") == 0 
			|| ft_strcmp(node->type, "heredoc") == 0)
		{
			if (!parse_redir_node(&node, tail))
				return (cleanup_redirection(redir_list), NULL);
			print_redir_list(&redir_list);
			while (node && (!ft_strcmp(node->type, "heredoc") \
			|| !ft_strcmp(node->type, "delimiter")))
				node = node->right;
		}
		else
			node = node->right;
	}
	return (redir_list);
}
