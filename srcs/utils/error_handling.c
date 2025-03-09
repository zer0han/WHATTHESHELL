/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:51:24 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/09 18:47:12 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*error_handling functions
* error in args
* error in builtins
* wrong cmds error
* ...
*/

void	handle_error(char *context, int errnum, t_token **tokens)
{
	ft_putstr_fd("whattheshell: ", STDERR_FILENO);
	ft_putstr_fd(context, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errnum), STDERR_FILENO);
	free_tokens(*tokens);
	exit(EXIT_FAILURE);
}

int	error_message(char *context, int error_code)
{
	ft_putstr_fd("whattheshell: ", STDERR_FILENO);
	ft_putstr_fd(context, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(error_code), STDERR_FILENO);
	return (1);
}

void	free_array(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

//close pipis function and cacas