/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:27:16 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/28 14:36:00 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;

	input = readline("Enter command: ");
	tokens = NULL;
	while (input != NULL)
	{
		if (*input)
			add_history(input);
		tokens = ft_tokenize(input);
		if (tokens == NULL)
			return (0);
		while (tokens)
		{
			printf("input : %s\n", tokens->input);
			printf("type : %s\n\n", tokens->type);
			tokens = tokens->right;
		}
		free(input);
		input = readline("Enter command: ");
	}
	free(input);
	return (0);
}
