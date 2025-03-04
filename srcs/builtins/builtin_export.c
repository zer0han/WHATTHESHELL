/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/04 16:52:57 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

/***cmd_export
*export
**to view all the exported variables.

*export -p
**to view all the exported variables on current shell

*export -f [function name]
**to export shell function
*Must be used if the names refer to functions.
*If -f is not used, the export will assume the names are variables.

*export -n [variable_name]
**Named variables (or functions, with -f) will no longer be exported
**No output will be seen on screen, to see exported variable grep from exported ones is used

*export without argument, the environment variables are displayed in alphabetical order.
**each variable is printed in the format
** declare -x VAR="value"
**sorting the environment ensures consistent behavior when user calls export

*export with arguments
**if args passed like export VAR=value, the variable is added or updated in the environment list without
needing sorting immediately

output

declare -x HOME="/home/user"
declare -x PATH="/usr/bin"
declare -x SHELL="/bin/bash"

***/

static char	*ft_join_env_var(char *var, char *value)
{
	char	*temp;
	char	*result;

	if (!var || !value)
		return (NULL);
	temp = ft_strjoin(var, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

void	sort_export_env(char **object)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (object[i] && object [i + 1])
	{
		j = i + 1;
		if(ft_strcmp(object[i], object[j]) > 0)
		{
			temp = object[i];
			object[i] = object[j];
			object[j] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int	update_env(char ***envp, char *var, char *value)
{
	int		i;
	char	*new_entry;

	if (!envp || !var || !value)
		return (1);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp((*envp)[i], var, ft_strlen(var)) == 0 \
		&& ((*envp)[i][ft_strlen(var)] == '=' || \
		(*envp)[i][ft_strlen(var)] == '\0'))
		{
			new_entry = ft_join_env_var(var, value);
			if (!new_entry)
				return (1);
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return (0);
		}
		i++;
	}
	return (1);
}

int	add_env(char ***envp, char *var, char *value)
{
	int		count;
	int		i;
	char	**new_env;
	char	*new_entry;

	if (!envp || !var || !value)
		return (1);
	count = 0;
	while ((*envp)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup((*envp)[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			free (new_env);
			return (1);
		}
		i++;
	}
	new_entry = ft_join_env_var(var, value);
	if (!new_entry)
		return (free(new_env), 1);
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	//free(*envp);
	*envp = new_env;
	printf("DEBUGGGGGG:PleAsE ADD THE neW VARiable %s=%s\n", var, value);
	return (0);
}


int	cmd_export(char ***envp, t_token *tokens)
{
	int		j;
	char	*equal_sign;
	char	*var;
	char	*value;
	t_token	*arg;

	if (!tokens || !tokens->right)
	{
		j = 0;
		sort_export_env(*envp);
		while ((*envp)[j])
			printf("export %s\n", (*envp)[j++]);
		return (0);
	}
	arg = tokens->right;
	while (arg)
	{
		equal_sign = ft_strchr(arg->input, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			var = ft_strdup(arg->input);
			value = equal_sign + 1;
			if (!valid_id(var))
				return (printf("export: not a valid arg\n"), free(var), 1);
			if (update_env(envp, var, value))
			{
				if (add_env(envp, var, value))
					return (free(var), 1);
			}
			free(var);
		}
		else if (!valid_id(arg->input))
			return (printf("export: not a valid arg\n"), 1);
		arg = arg->right;
	}
	return (0);
}

