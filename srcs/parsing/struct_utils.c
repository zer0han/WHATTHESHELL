/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:40:18 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/23 20:41:11 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


t_token	*create_node(t_token **tokens, char *char_token)
{
	t_token	*node;
	t_token	*last;

	node = ft_calloc(1, sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->input = ft_strdup(char_token);
	node->type = NULL;
	if (!(*tokens))
	{
		*tokens = node;
		node->left = NULL;
	}
	else
	{
		last = ft_lastnode(*tokens);
		last->right = node;
		node->left = last;
	}
	node->right = NULL;
	return (*tokens);
}

t_token	*ft_lastnode(t_token *tokens)
{
	if (tokens == NULL)
		return (NULL);
	while (tokens->right != NULL && tokens != NULL)
		tokens = tokens->right;
	return (tokens);
}

t_token	*is_special_str(t_token **tokens, char *str)
{
	t_token	*node;

	node = *tokens;
	while (node)
	{
		if (!ft_strncmp(node->input, str, ft_strlen(node->input)))
			return (node);
		node = node->right;
	}
	return (NULL);
}
