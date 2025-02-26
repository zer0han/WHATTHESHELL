/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:40:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/02/26 18:43:52 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char				*input;
	t_token				*tokens;
	t_exec				*exec_data;
	struct sigaction	*sa;

	(void)argc;
	(void)argv;
	input = readline("WHATTHESHELL: ");
	tokens = NULL;
	while (input != NULL)
	{
		if (*input)
			add_history(input);
		tokens = ft_minishell_parsing(input);
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
