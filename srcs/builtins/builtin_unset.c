/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:13:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/16 11:01:56 by gmechaly         ###   ########.fr       */
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

static void	remove_clean_env(t_envp **env, t_envp *node)
{
	if (node->next && node->prev)
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
	else if (node->next && !node->prev)
	{
		node->next->prev = NULL;
		env = &node->next;
	}
	else if (node->prev && !node->next)
		node->prev->next = NULL;
}

int	env_remover(t_envp **env, char *var)
{
	int		var_len;
	t_envp	*node;

	node = *env;
	if (!var || !valid_id(var))
		return (1);
	var_len = ft_strlen(var);
	while (node)
	{
		if (ft_strncmp(node->str, var, var_len) == 0 && (node->str[var_len] == '=' \
			|| node->str[var_len] == '\0'))
		{
			free(node->str);
			remove_clean_env(env, node);
			free(node);
			return (0);
		}
		node = node->next;
	}
	return (0);
}

int	cmd_unset(t_envp *env, t_token *tokens)
{
	t_token	*arg;

	if (!tokens || !tokens->right || !env)
		return (1);
	arg = tokens->right;
	while (arg)
	{
		env_remover(&env, arg->input);
		arg = arg->right;
	}
	return (0);
}