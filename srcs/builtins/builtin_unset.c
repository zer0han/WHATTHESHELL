/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:13:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/29 19:33:52 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/***unset***/
/* unset [variable_name]
 * unset [-options] [variable_name]
 ** undefine a variable in bash
 * unset -f [function_name]
 ** undefine shell function in bash
*/

int	valid_id(char *var)
{
	if (!var || (!(*var >= 'A' && *var <= 'Z') && \
		!(*var >= 'a' && *var <= 'z')))
		return (0);
	var++;
	while (*var)
	{
		if (!((*var >= 'A' && *var <= 'Z') || !(*var >= 'a' && *var <= 'z')) \
			|| (*var >= '0' && *var <= '9') || (*var == '_'))
			return (0);
		var++;
	}
	return (1);
}

int	cmd_unset(char **envp, char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (1);
	if (!valid_id(var))
		return (ft_printf(stderr, "unset: '%s': not a valid  identifier\n", \
			var), 1);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0 && \
			envp[i][ft_strlen(var)] == '=')
		{
			free (envp[i]);
			while (envp[i])
			{
				envp[i] = envp[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}
