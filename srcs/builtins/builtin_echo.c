/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:48:47 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 12:59:46 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cmd_echo(t_token *tokens)
{
	int		nl;
	int		j;
	t_token	*args;

	nl = 1;
	if (!tokens || ft_strcmp(tokens->input, "echo") != 0)
		return (1);
	args = tokens->right;
	if (tokens->right->input[0] == '-')
	{
		j = 1;
		while (args->input[j] && args->input[j] == 'n')
			j++;
		if (args->input[j] == '\0')
		{
			nl = 0;
			args = args->right;
		}
	}
	while (args)
	{
		printf("%s", args->input);
		if (args->right)
			printf(" ");
		args = args->right;
	}
	if (nl)
		printf("\n");
	return (0);
}
