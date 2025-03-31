/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:07:48 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 22:22:10 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	cmd_exit(t_token *token, t_exec *exec_list, t_envp *env)
{
	int	exit_code;

	exit_code = 0;
	if (token && token->right && token->right->right)
	{
		ft_putstr_fd("whattheshell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
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
	close(exec_list->std_save[0]);
	close(exec_list->std_save[1]);
	free_all(token, exec_list);
	free_envp(env);
	exit(exit_code);
}
