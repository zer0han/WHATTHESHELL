/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:13:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/06 17:27:12 by rdalal           ###   ########.fr       */
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
	if (!var || (!((*var >= 'A' && *var <= 'Z') || \
		(*var >= 'a' && *var <= 'z'))))
		return (0);
	var++;
	while (*var)
	{
		if (!((*var >= 'A' && *var <= 'Z') || (*var >= 'a' && *var <= 'z') \
			|| (*var >= '0' && *var <= '9') || (*var == '_')))
			return (0);
		var++;
	}
	return (1);
}

int	env_remover(char **envp, char *var)
{
	int	i;
	int var_len;

	i = 0;
	var_len = ft_strlen(var);
	if (!var || !valid_id(var))
		return(EXIT_FAILURE);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && (envp[i][var_len] == '=' \
			|| envp[i][var_len] == '\0'))
		{
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
	return (0);
}

int	cmd_unset(char ***envp, t_token *tokens)
{
	t_token	*arg;

	if (!tokens || !tokens->right)
		return (EXIT_FAILURE);
	arg = tokens->right;
	while (arg)
	{
		env_remover(*envp, arg->input);
		arg = arg->right;
	}
	return (EXIT_SUCCESS);
}