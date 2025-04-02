/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:07:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/02 18:13:56 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_envp	*ft_last_env_node(t_envp **duplicate)
// {
// 	t_envp	*node;

// 	node = *duplicate;
// 	if (node == NULL)
// 		return (NULL);
// 	while (node && node->next)
// 		node = node->next;
// 	return (node);
// }

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

static int	update_env(t_envp **dup, char *var, char *val)
{
	t_envp	*nv;
	int		i;

	nv = *dup;
	i = 0;
	while (nv)
	{
		if (strncmp(nv->str, var, ft_strlen(var)) == 0 && \
			nv->str[ft_strlen(var)] == '=')
		{
			free(nv->str);
			nv->str = malloc(ft_strlen(var) + ft_strlen(val) + 2);
			if (nv->str == NULL)
				return (-1);
			ft_strlcpy(nv->str, var, ft_strlen(var) + 1);
			ft_strlcat(nv->str, "=", ft_strlen(nv->str) + 2);
			ft_strlcat(nv->str, val, ft_strlen(nv->str) + ft_strlen(val) + 2);
			if (is_quote(nv->str[ft_strlen(nv->str) - 1]))
				nv->str[ft_strlen(nv->str) - 1] = '\0';
			return (0);
		}
		nv = nv->next;
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
