/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/12 22:34:19 by gmechaly         ###   ########.fr       */
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
// }*/
// static char	**dup_env(char **envp)
// {
// 	char	**copy;
// 	int		count;
// 	int		i;

// 	count = 0;
// 	while (envp[count])
// 		count++;
// 	copy = malloc(sizeof(char *) * (count + 1));
// 	if (!copy)
// 		return (NULL);
// 	i = -1;
// 	while (++i < count)
// 		copy[i] = ft_strdup(envp[i]);
// 	copy[count] = NULL;
// 	return (copy);
// }
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
	void	*t;

	t = malloc(1);
	if (!ptr)
		return (0);
	free (t);
	if (ptr >= t & ptr <= (t + 1))
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

// static void	print_env(t_envp **dup)
// {
// 	t_envp	*node;

// 	node = *dup;
// 	while (node)
// 	{
// 		printf("%s\n", node->str);
// 		node = node->next;
// 	}
// }

// static t_envp	*ft_last_env_node(t_envp *duplicate)
// {
// 	if (duplicate == NULL)
// 		return (NULL);
// 	while (duplicate && duplicate->next)
// 		duplicate = duplicate->next;
// 	return (duplicate);
// }

// static t_envp	*create_envp_node(char *env_line, t_envp **duplicate)
// {
// 	t_envp	*node;

// 	node = ft_calloc(1, sizeof(t_envp));
// 	if (node == NULL)
// 		return (NULL);
// 	node->str = ft_strdup(env_line);
// 	if (!(*duplicate))
// 	{
// 		duplicate = &node;
// 		node->prev = NULL;
// 	}
// 	else
// 	{
// 		node->prev = ft_last_env_node(*duplicate);
// 		node->prev->next = node;
// 	}
// 	node->next = NULL;
// 	return (*duplicate);
// }

// t_envp	*envp_dup(char **envp)
// {
// 	t_envp	*dup_env;
// 	int		i;

// 	i = 0;
// 	dup_env = NULL;
// 	while (envp[i])
// 	{
// 		dup_env = create_envp_node(envp[i], &dup_env);
// 		if (dup_env == NULL)
// 			return (free_envp(dup_env), NULL);
// 		i++;
// 	}
// 	return(dup_env);
// }

// static void	*add_env(t_envp **dup, char *var, char *value)
// {
// 	char	*new_line;

// 	new_line = ft_strdup(var);
// 	if (new_line == NULL)
// 		return (NULL);
// 	ft_strlcat(new_line, "=", ft_strlen(new_line + 1));
// 	ft_strlcat(new_line, value, ft_strlen(new_line) + ft_strlen(value) + 1);
// 	if (create_envp_node(new_line, dup) == NULL)
// 		return (NULL);
// 	return (*dup);
// }

// static int	update_env(t_envp **dup, char *var, char *value)
// {
// 	t_envp	*node;

// 	node = *dup;
// 	while (node)
// 	{
// 		if (!ft_strncmp((*envp)[i], var, ft_strlen(var)) && 
// 			((*envp)[i][ft_strlen(var)] == '=' || 
// 			(*envp)[i][ft_strlen(var)] == '\0'))
// 		{
// 			free(node->str);
// 			node->str = malloc(ft_strlen(var) + ft_strlen(value) + 2);
// 			if (node->str == NULL)
// 				return (-1);
// 			ft_strlcpy(node->str, var, ft_strlen(var));
// 			ft_strlcat(node->str, "=", ft_strlen(node->str) + 1);
// 			ft_strlcat(node->str, value, ft_strlen(node->str) + ft_strlen(value));
// 			return (0);
// 		}
// 		node = node->next;
// 	}
// 	return (1);
// }

// static void	process_export_arg(char *arg, t_envp **dup)
// {
// 	char	*eq;
// 	char	*var;
// 	char	*value;

// 	eq = ft_strchr(arg, '=');
// 	if (eq)
// 	{
// 		var = ft_substr(arg, 0, eq - arg);
// 		if (!var)
// 			exit(EXIT_FAILURE);
// 		value = eq + 1;
// 		if (!valid_id(var))
// 		{
// 			free(var);
// 			exit(EXIT_FAILURE);
// 		}
// 		if (update_env(dup, var, value) > 0)
// 			add_env(dup, var, value);
// 		free(var);
// 	}
// 	else if (!valid_id(arg))
// 		exit(EXIT_FAILURE);
// 	exit(EXIT_SUCCESS);
// }

void	print_env(t_envp **dup)
{
	t_envp	*node;

	node = *dup;
	while (node)
	{
		printf("%s\n", node->str);
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
			return (/*free_envp(dup_env),*/ NULL);
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
	*dup = create_envp_node(new_line, dup);
	if (*dup == NULL)
		return (printf("didn't create the node\n"), NULL);
	return (printf("node created\n"), *dup);
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
			ft_strlcpy(node->str, var, ft_strlen(var));
			ft_strlcat(node->str, "=", ft_strlen(node->str) + 1);
			ft_strlcat(node->str, value, ft_strlen(node->str) + ft_strlen(value));
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
		if (!valid_id(var))
			return (free(var), EXIT_FAILURE);
		printf("\nlast node = %s\n", ft_last_env_node(dup)->str);
		if (update_env(dup, var, value) > 0)
			add_env(dup, var, value);
		free(var);
		// print_env(dup);
	}
	else if (!valid_id(arg))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	cmd_export(t_exec *exec, t_token **tokens)
{
	t_token	*node;
	int		retval;

	(void)exec;
	node = *tokens;
	if (!node->right)
	{
		print_env(&exec->envp);
		return (EXIT_SUCCESS);
	}
	printf("\nBefore export\n");
	print_env(&exec->envp);
	node = node->right;
	while (node && !ft_strncmp(node->type, "arg", 3))
	{
		retval = process_export_arg(node->input, &exec->envp);
		if (retval == EXIT_FAILURE)
			return (EXIT_FAILURE);
		node = node->right;
	}
	return (EXIT_SUCCESS);
}
