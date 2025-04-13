/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:07:48 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/13 17:37:10 by rdalal           ###   ########.fr       */
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
	int		exit_code;

	exit_code = 0;
	if (token && token->right)
	{
		if (exitcode_check(token->right->input))
		{
			ft_putstr_fd(PROMPT "exit: ", STDERR_FILENO);
			ft_putstr_fd(token->right->input, STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit_code = 2;
		}
		else if (token->right->right)
			return (ft_putendl_fd(PROMPT "exit: too many arguments", \
				STDERR_FILENO), 1);
		else
			exit_code = ft_atoi(token->right->input) % 256;
	}
	printf("exit\n");
	close(exec_list->std_save[0]);
	close(exec_list->std_save[1]);
	free_all(token, exec_list);
	free_envp(env);
	g_exit_status = exit_code;
	exit(exit_code);
}
