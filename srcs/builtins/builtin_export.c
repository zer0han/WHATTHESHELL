/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/28 17:26:03 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdlib.h>
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

void	print_env(t_envp **dup)
{
	t_envp	*node;

	node = *dup;
	while (node)
	{
		printf("export %s\n", node->str);
		node = node->next;
	}
}

static t_envp	*ft_last_env_node(t_envp **duplicate)
{
	t_envp	*node;

	node = *duplicate;
	if (node == NULL)
		return (NULL);
	while (node && node->next)
		node = node->next;
	return (node);
}

static t_envp	*create_envp_node(char *env_line, t_envp **duplicate)
{
	t_envp	*node;

	node = ft_calloc(1, sizeof(t_envp));
	if (node == NULL)
		return (NULL);
	node->str = ft_strdup(env_line);
	if (!(*duplicate))
	{
		duplicate = &node;
		node->prev = NULL;
	}
	else
	{
		node->prev = ft_last_env_node(duplicate);
		node->prev->next = node;
	}
	node->next = NULL;
	return (*duplicate);
}

t_envp	*envp_dup(char **envp)
{
	t_envp	*dup_env;
	int		i;

	i = 0;
	dup_env = NULL;
	while (envp[i])
	{
		dup_env = create_envp_node(envp[i], &dup_env);
		if (dup_env == NULL)
			return (NULL);
		i++;
	}
	return(dup_env);
}

static void	*add_env(t_envp **dup, char *var, char *value)
{
	char	*new_line;

	new_line = malloc(sizeof(char) * ft_strlen(var) + ft_strlen(value) + 2);
	if (new_line == NULL)
		return (NULL);
	ft_strlcpy(new_line, var, ft_strlen(var) + 1);
	ft_strlcat(new_line, "=", ft_strlen(new_line) + 2);
	ft_strlcat(new_line, value, ft_strlen(new_line) + ft_strlen(value) + 2);
	if (is_quote(new_line[ft_strlen(new_line) - 1]))
		new_line[ft_strlen(new_line) - 1] = '\0';
	*dup = create_envp_node(new_line, dup);
	free(new_line);
	if (*dup == NULL)
		return (NULL);
	return (*dup);
}

static int	update_env(t_envp **dup, char *var, char *value)
{
	t_envp	*node;
	int		i;

	node = *dup;
	i = 0;
	while (node)
	{
		if (strncmp(node->str, var, ft_strlen(var)) == 0 && \
			node->str[ft_strlen(var)] == '=')
		{
			free(node->str);
			node->str = malloc(ft_strlen(var) + ft_strlen(value) + 2);
			if (node->str == NULL)
				return (-1);
			ft_strlcpy(node->str, var, ft_strlen(var) + 1);
			ft_strlcat(node->str, "=", ft_strlen(node->str) + 2);
			ft_strlcat(node->str, value, ft_strlen(node->str) + ft_strlen(value) + 2);
			if (is_quote(node->str[ft_strlen(node->str) - 1]))
				node->str[ft_strlen(node->str) - 1] = '\0';
			return (0);
		}
		node = node->next;
		i++;
	}
	return (1);
}

static int	process_export_arg(char *arg, t_envp **dup)
{
	char	*eq;
	char	*var;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		var = ft_substr(arg, 0, eq - arg);
		if (!var)
			exit(EXIT_FAILURE);
		value = eq + 1;
		if (is_quote(*value))
			value++;
		if (!valid_id(var))
			return (free(var), EXIT_FAILURE);
		if (update_env(dup, var, value) > 0)
		{
			if (add_env(dup, var, value) == NULL)
				return (free(var), EXIT_FAILURE);
		}
		free(var);
	}
	else if (!valid_id(arg))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	cmd_export(t_envp *env, t_token **tokens)
{
	t_token	*node;
	int		retval;

	node = *tokens;
	if (!node->right)
	{
		print_env(&env);
		return (EXIT_SUCCESS);
	}
	node = node->right;
	while (node && !ft_strncmp(node->type, "arg", 3))
	{
		retval = process_export_arg(node->input, &env);
		if (retval == EXIT_FAILURE)
			return (EXIT_FAILURE);
		node = node->right;
	}
	return (EXIT_SUCCESS);
}
