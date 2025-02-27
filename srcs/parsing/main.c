/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:27:16 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/27 13:11:41 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	ft_print_tokens(t_token **tokens)
// {
// 	t_token	*node;
// 	int		i;

// 	i = 0;
// 	node = *tokens;
// 	while (node)
// 	{
// 		printf("input %d : %s\n", i, node->input);
// 		printf("type : %s\n\n", node->type);
// 		node = node->right;
// 		i++;
// 	}
// }

t_token	*ft_minishell_parsing(char *input)
{
	char	*new_input;
	t_token	*tokens;

	new_input = expand_variables(input);
	free(input);
	if (new_input == NULL)
		return (NULL);
	tokens = ft_tokenize(new_input);
	free(new_input);
	if (!tokens || !parse_tokens(&tokens) || !is_null_token(&tokens))
		return (NULL);
	return (tokens);
}
