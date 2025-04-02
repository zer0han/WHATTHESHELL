/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:40:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/02 16:28:38 by rdalal           ###   ########.fr       */
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


int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_exec		*exec_data;
	t_envp		*env_linklist;

	(void)argc;
	(void)argv;
	signals();
	input = readline("WHATTHESHELL: ");
	tokens = NULL;
	env_linklist = envp_dup(envp);
	while (input != NULL)
	{
		if (*input)
			add_history(input);
		tokens = ft_minishell_parsing(input, env_linklist);
		//ft_print_tokens(&tokens);
		if (tokens)
		{
			exec_data = main_execution(&tokens, env_linklist);
			free_all(tokens, exec_data);
		}
		printf("readline ? is that you??\n");
		input = readline("WHATTHESHELL: ");
	}
	printf("main got out of its loop zebi\n");
	free(input);
	free_envp(env_linklist);
	return (0);
}
