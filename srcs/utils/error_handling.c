/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:51:24 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/24 16:02:28 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*error_handling functions
* error in args
* error in builtins
* wrong cmds error
* ...
*/

void	free_shell(t_token **cmd_line)
{
	t_token	*temp;
	t_token	*current;

	if (!cmd_line)
		return ;
	current = *cmd_line;
	while (current)
	{
		temp = current->next;
		current->value = 0;
		free(current);
		current = temp;
	}
	*cmd_line = NULL;
}

void	free_errors(t_token **cmd_line)
{
	if (cmd_line && *cmd_line)
	{
		free_shell(cmd_line);
	}
	ft_printf("error\n");
	exit (1);
}