/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:13:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/03 18:12:05 by rdalal           ###   ########.fr       */
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
	if (!var || !((*var >= 'A' && *var <= 'Z') || \
		!(*var >= 'a' && *var <= 'z')))
		return (0);
	var++;
	while (*var)
	{
		if (!((*var >= 'A' && *var <= 'Z') || !(*var >= 'a' && *var <= 'z') \
			|| (*var >= '0' && *var <= '9') || (*var == '_')))
			return (0);
		var++;
	}
	return (1);
}

int	cmd_unset(char **envp, char *var)
{
	int	i;
	int var_len;

	i = 0;
	var_len = ft_strlen(var);
	if (!var)
		return (1);
	if (!valid_id(var))
		return(1);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && (envp[i][var_len] == '=' \
			|| envp[i][var_len] == '\0'))
		{
			printf("match found: \"%s\" for the var \"%s\"\n", envp[i], var); //debug check
			free(envp[i]);
			while (envp[i])
			{
				envp[i] = envp[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	printf("variable \"%s\" not found in envp, \n", var); //debug check
	return (0);
}
