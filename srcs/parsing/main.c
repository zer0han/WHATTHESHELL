/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:27:16 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/13 15:24:05 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_print_tokens(t_token **tokens)
{
	t_token	*node;
	int		i;

	i = 0;
	node = *tokens;
	while (node)
	{
		printf("input %d : %s\n", i, node->input);
		printf("type : %s\n\n", node->type);
		node = node->right;
		i++;
	}
}

t_token	*ft_minishell_parsing(char *input)
{
	char	*new_input;
	t_token	*tokens;

	new_input = expand_variables(input);
	free(input);
	tokens = ft_tokenize(new_input);
	free_tokens(tokens);
	free(new_input);
	if (tokens == NULL || parse_tokens(&tokens) == NULL)
		return (NULL);
	return (tokens);
}

int	main(void)
{
	char	*input;
	char	*ninput;
	t_token	*tokens;

	input = readline("Enter command: ");
	tokens = NULL;
	ninput = NULL;
	while (input != NULL)
	{
		if (*input)
			add_history(input);
		ninput = expand_variables(input);
		if (ninput == NULL)
			return (free(input), 0);
		free(input);
		tokens = ft_tokenize(ninput);
		if (tokens == NULL || parse_tokens(&tokens) == NULL)
			return (free_tokens(tokens), free(ninput), 0);
		ft_print_tokens(&tokens);
		free_tokens(tokens);
		free(ninput);
		input = readline("Enter command: ");
	}
	free(input);
	return (0);
}

/*< infile cat | wc -l -c > outfile -> segfault*/
