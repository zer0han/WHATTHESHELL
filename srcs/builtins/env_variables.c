/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:17:17 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/20 13:23:41 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*for handling the environment variables $ followed by a sequence of chars*/

/***env***/

/*
*if no flags or parameters specified, the env command displays your current environment, showing one Name=Value per line
**Print out a list of all the env variables

*'-i' or '-ignore-environment' or '-'
**runs a command with an empty environment

[-i | -] [Name=Value]... [Command [Argument ...]]
*/

int	cmd_env(char **argv, char **envp)
{
	char	*env_variable;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	env_variable = envp[i];
	while (env_variable)
	{
		ft_printf("%s\n", env_variable);
		key = strtok(env_variable, "=");
		value = strtok(NULL, "=");
		// check about this, it just checks for the PATH variable find a way for all env variables
		if (ft_strcmp(key, "PATH") == 0)
			ft_printf("%s\n", value);
		i++;
		env_variable = envp[i];
	}
	return (0);
}

//env utils here there are more functions that we need here, check git and ask around