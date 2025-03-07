/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:40:07 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/07 22:18:48 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_node(t_token **tokens, char *char_token)
{
	t_token	*node;

	node = ft_calloc(1, sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->input = ft_strdup(char_token);
	node->type = NULL;
	if (!(*tokens))
	{
		tokens = &node;
		node->left = NULL;
	}
	else
	{
		node->left = ft_lastnode(*tokens);
		node->left->right = node;
	}
	node->right = NULL;
	return (*tokens);
}

t_token	*ft_tokenize(char *input)
{
	char	**char_tokens;
	int		i;
	t_token	*tokens;

	char_tokens = ft_split_for_tokens(input);
	if (char_tokens == NULL)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (char_tokens[i] != NULL)
	{
		tokens = create_node(&tokens, char_tokens[i]);
		if (tokens == NULL)
			return (free_string_tab(&char_tokens[i]), free_tokens(tokens), NULL);
		free(char_tokens[i]);
		i++;
	}
	free (char_tokens);
	if (assign_token_type(&tokens) == NULL)
		return (NULL);
	assign_missing_cmds(&tokens);
	assign_options_and_args(&tokens);
	return (tokens);
}
