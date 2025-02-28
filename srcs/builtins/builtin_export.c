/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/28 17:36:22 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
			j = 0;
		}
		else
			i++;
	}
}

int	update_env(char **envp, char *var, char *value)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		//printf("envp check here[%d]: %s\n", i, envp[i]);
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0 && \
			envp[i][ft_strlen(var)] == '=')
		{
			printf("updating var %s with the value: %s\n", var, value);
			free (envp[i]);
			envp[i] = malloc(ft_strlen(var) + ft_strlen(value) + 2);
			if (!envp[i])
				return (1);
			sprintf(envp[i], "%s=%s", var, value);
			printf("updating envp[%d]: %s\n", i,  envp[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

void	fd_print_env(char **envp)
{
	int	i = 0;
	while (envp[i])
	{
		printf("envp: %s\n", envp[i]);
		i++;
	}
}

int	add_env(char ***envp, char *var, char *value)
{
	int		count;
	int		i;
	char	**new_env;
	char	**old_env;

	count = 0;
	old_env = *envp;
	while ((*envp)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup((*envp)[i]);
		i++;
	}
	new_env[count] = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_env[count])
		return (1);
	sprintf(new_env[count], "%s=%s", var, value);
	new_env[count + 1] = NULL;
	*envp = new_env;
	//free(*old_env);
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
		while ((*envp)[j])
		{
			sort_export_env(*envp);
			printf("export %s\n", (*envp)[j++]);
		}
		return (0);
	}
	arg = tokens->right;
	while (arg)
	{
		equal_sign = ft_strchr(arg->input, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			var = arg->input;
			value = equal_sign + 1;
			if (!valid_id(var))
				return (printf("export: not a valid argument\n"), 1);
			//fd_print_env(*envp);
			if (update_env(*envp, var, value))
				add_env(envp, var, value);
			//fd_print_env(*envp);
		}
		else
		{
			if (!valid_id(arg->input))
				return (printf("export: not a valid argument"), 1);
		}
		arg = arg->right;
	}
	return (0);
}

