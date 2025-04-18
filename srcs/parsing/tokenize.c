/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:40:07 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:45:11 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_tokenize(char *input, t_envp *env)
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
			return (free_string_tab(&char_tokens[i]), \
			free_tokens(tokens), NULL);
		free(char_tokens[i]);
		i++;
	}
	free (char_tokens);
	if (assign_token_type(&tokens) == NULL)
		return (free_tokens(tokens), NULL);
	assign_missing_cmds(&tokens, env);
	assign_options_and_args(&tokens);
	return (tokens);
}
