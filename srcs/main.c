/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:40:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/27 20:11:58 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	ft_print_tokens(t_token **tokens)
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
}*/

int	main(int argc, char **argv, char **envp)
{
	char				*input;
	t_token				*tokens;
	t_exec				*exec_data;
	//struct sigaction	*sa;

	(void)argc;
	(void)argv;
	input = readline("WHATTHESHELL: ");
	tokens = NULL;
	while (input != NULL)
	{
		if (*input)
			add_history(input);
		tokens = ft_minishell_parsing(input);
		//ft_print_tokens(&tokens);
		if (tokens)
		{
			exec_data = main_execution(&tokens, envp);
			free_all(tokens, exec_data);
		}
		input = readline("WHATTHESHELL: ");
	}
	free(input);
	return (0);
}
