/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/05 23:05:10 by rdalal           ###   ########.fr       */
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

/*static char	*ft_join_env_var(char *var, char *value)
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
}*/
static char	**dup_env(char **envp)
{
	char	**copy;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (++i < count)
		copy[i] = ft_strdup(envp[i]);
	copy[count] = NULL;
	return (copy);
}

static void	print_sorted_env(char **envp)
{
	char	**copy;
	int		i;
	int		j;
	char	*temp;

	copy = dup_env(envp);
	if (!copy)
		exit(EXIT_FAILURE);
	i = -1;
	while (copy[++i])
	{
		j = i;
		while (copy[++j])
		{
			if (ft_strcmp(copy[i], copy[j]) > 0)
			{
				temp = copy[i];
				copy[i] = copy[j];
				copy[j] = temp;
			}
		}
	}
	i = -1;
	while (copy[++i])
		printf("export %s\n", copy[i]);
	free_array(copy);
}

static char	*create_env_entry(char *var, char *value)
{
	char	*entry;
	int		var_len;
	int		val_len;

	if (!var || !value)
		return (NULL);
	var_len = ft_strlen(var);
	val_len = ft_strlen(value);
	entry = malloc(var_len + val_len + 2);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, var, var_len + 1);
	entry[var_len] = '=';
	ft_strlcpy(entry + var_len + 1, value, val_len + 1);
	return (entry);
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

static int	is_allocated(void *ptr)
{
	void	*test;

	test = malloc(1);
	if (!ptr)
		return (0);
	free (test);
	if (ptr >= test & ptr <= (test + 1))
		return (0);
	return (1);
}

int	update_env(char ***envp, char *var, char *value)
{
	int		i;
	char	*new_entry;

	if (!envp || !var || !value)
		return (1);
	i = -1;
	while ((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], var, ft_strlen(var)) && \
			((*envp)[i][ft_strlen(var)] == '=' || \
			(*envp)[i][ft_strlen(var)] == '\0'))
		{
			new_entry = create_env_entry(var, value);
			if (!new_entry)
				return (1);
			if (is_allocated((*envp)[i]))
				free((*envp)[i]);
			(*envp)[i] = new_entry;
			return (0);
		}
	}
	return (1);
}

int	add_env(char ***envp, char *var, char *value)
{
	int		count;
	int		i;
	char	**new;
	char	*entry;

	if (!envp|| !var || !value)
		return (1);
	count = 0;
	while ((*envp)[count])
		count++;
	entry = create_env_entry(var, value);
	if (!entry)
		return (1);
	new = malloc(sizeof(char *) * (count + 2));
	if (!new)
		return (free(entry), 1);
	i = -1;
	while (++i < count)
		new[i] = ft_strdup((*envp)[i]);
	new[count] = entry;
	new[count + 1] = NULL;
	//free_array((*envp));
	*envp = new;
	return (0);
}

static int	process_export_arg(char *arg, char ***envp)
{
	char	*eq;
	char	*var;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		var = ft_substr(arg, 0, eq - arg);
		value = eq + 1;
		if (!valid_id(var))
			return (free(var), EXIT_FAILURE);
		if (update_env(envp, var, value))
		{
			if (add_env(envp, var, value))
				return (free(var), EXIT_FAILURE);
		}
		free(var);
	}
	else if (!valid_id(arg))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	cmd_export(char ***envp, t_token *tokens)
{
	t_token	*arg;

	if (!tokens->right)
		return (print_sorted_env(*envp), EXIT_SUCCESS);
	arg = tokens->right;
	while (arg)
	{
		if (process_export_arg(arg->input, envp))
			return (EXIT_FAILURE);
		arg = arg->right;
	}
	return (EXIT_SUCCESS);
}

