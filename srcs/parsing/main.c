/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:27:16 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:01:08 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*ft_minishell_parsing(char *input, t_envp *env)
{
	char	*new_input;
	t_token	*tokens;

	new_input = expand_variables(input, env);
	free(input);
	if (new_input == NULL)
		return (NULL);
	tokens = ft_tokenize(new_input, env);
	free(new_input);
	if (!tokens || !parse_tokens(&tokens, env) || !is_null_token(&tokens))
		return (free_tokens(tokens), NULL);
	return (tokens);
}
