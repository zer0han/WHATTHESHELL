/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:07:48 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/03 21:53:58 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exitcode_check(char *code)
{
	char	*temp;
	char	nbr;

	temp = code;
	if ((*temp == '+' || *temp == '-') && *(temp + 1))
		temp++;
	while (*temp)
	{
		if (!ft_isdigit(*temp++))
			return (1);
	}
	nbr = ft_atol(code);
	if ((nbr > 0 && (LONG_MAX / nbr < 1)) || \
		(nbr < 0 && (LONG_MIN / ft_atol(code) < 1)))
		return (1);
	return (0);
}

/***builtin exit***/
/* exit [N (exit_nbr)]
	check if there are more than one arguments and if the first argument is not a valid exit code
	print error if not the correct number of arguments and return with exit code 1
	set default exit code at 0
	check if the first argument is a valid exit code(should be a nbr)
	if valid convert the arg to an int
	if arg is invalid, call exit error function to handle
	set exit code to 2 to indicate an error
	close any open resources in the shell context
	free memory in the shell context
	exit the shell with the determined exit code
	*/

	/*
	will have to make another function to check if the provided value can be a valid exit code
	and ensure the value is a nbr and is in the valid range of the exit codes.
	can be something like 
	int exitcode_check(char *value)
*/

int	cmd_exit(t_token *code, t_token *args)
{
	int	exit_code;

	if (args && args->next && !exitcode_check(args->value))
		return(ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO), 1);
	exit_code = 0;
	if (args && !exitcode_check(args->value))
		exit_code = ft_atoi(args->value);
	else if (args && exitcode_check(args->value))
	{
		exitcode_check (args->value);
		exit_code = 2;
	}
	close(code);
	//free all shell or tokens here(code)
	exit(exit_code);
}