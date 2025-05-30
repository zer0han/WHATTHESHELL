/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:48:47 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/08 18:50:28 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_n_flag(t_token **args, int *nl)
{
	int	j;

	if (!(*args) || !(*args)->input || (*args)->input[0] != '-')
		return (0);
	j = 1;
	if ((*args)->input[j] != 'n')
		return (0);
	while ((*args)->input[j] == 'n')
		j++;
	if ((*args)->input[j] != '\0')
		return (0);
	*nl = 0;
	*args = (*args)->right;
	return (1);
}

int	cmd_echo(t_token *tokens)
{
	int		nl;
	t_token	*args;

	nl = 1;
	if (!tokens || ft_strcmp(tokens->input, "echo") != 0)
		return (EXIT_FAILURE);
	args = tokens->right;
	if (args && !handle_n_flag(&args, &nl))
	{
		printf("%s", args->input);
		args = args->right;
		if (args && (ft_strcmp(args->type, "arg") == 0))
			printf(" ");
	}		
	while (args && (ft_strcmp(args->type, "arg") == 0))
	{
		printf("%s", args->input);
		args = args->right;
		if (args && (ft_strcmp(args->type, "arg") == 0))
			printf(" ");
	}
	if (nl)
		printf("\n");
	return (EXIT_SUCCESS);
}
