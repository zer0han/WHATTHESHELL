/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:07:48 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/27 12:26:49 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	exitcode_check(char *code)
{
	char	*temp;
	long	nbr;

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

void	free_shell(t_data *ctx)
{
	t_token	*temp;
	t_token	*current;

	if (!ctx || !ctx->cmd)
		return ;
	current = ctx->cmd;
	while (current)
	{
		temp = current->next;
		free (current);
		current = temp;
	}
	if (ctx->args)
		free(ctx->args);
	free (ctx);
}

void	free_errors(t_token **cmd_line, const char *msg_error)
{
	if (cmd_line && *cmd_line)
		free_shell(*cmd_line);
	if (msg_error)
		ft_putstr_fd(msg_error, STDERR_FILENO);
	exit (1);
}

int	exit_error(char *input)
{
	ft_putstr_fd("whattheshell: exit: ", STDERR_FILENO);
	ft_putstr_fd(input, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	cmd_exit(t_data *ctx, t_token *args)
{
	int	exit_code;

	exit_code = 0;
	if (args && args->next && !exitcode_check(args->input))
	{
		ft_putstr_fd("whattheshell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (args && !exitcode_check(args->input))
		exit_code = ft_atoi(args->input);
	else if (args && exitcode_check(args->input))
	{
		exit_error(args->input);
		exit_code = 2;	
	}
	ft_close(args);
	free_shell(ctx);
	exit(exit_code);
}
void	ft_close(t_token *args)
{
	if (args && args->input)
		close(args->input);
	if (args && args->value)
	{
		free(args->value);
		args->value = NULL:
	}
}
