/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:07:48 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/02 19:48:54 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*int	exitcode_check(char *code)
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
}*/

int	exitcode_check(char *code)
{
	char	*temp;

	temp = code;
	if ((*temp == '+' || *temp == '-') && *(temp + 1))
		temp++;
	while (*temp)
	{
		if (!ft_isdigit(*temp))
			return (1);
		temp++;
	}
	return (0);
}

int	cmd_exit(t_token *token, t_exec *exec_list)
{
	int	exit_code;

	exit_code = 0;
	ft_putstr_fd("exit\n", STDIN_FILENO);
	if (token && token->right && token->right->right)
		return(ft_putstr_fd("whattheshell: exit: too many arguments\n", STDERR_FILENO), 1);
	if (token && token->right)
	{
		if (exitcode_check(token->right->input))
		{
			ft_putstr_fd("whattheshell: exit: nbr required\n", STDERR_FILENO);
			exit_code = 2;
		}
		else
			exit_code = ft_atoi(token->right->input) % 256;
	}	
	else
		exit_code = 0;
	free_all(token, exec_list);
	exit(exit_code);
}

//doesn't exit check this too