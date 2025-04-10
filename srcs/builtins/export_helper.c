/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:01:33 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/10 22:22:40 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	*update_shlvl(t_envp **env)
{
	char	*eq;
	int		lvl;
	char	*shlvl;
	t_envp	*node;

	node = *env;
	while (node && ft_strncmp("SHLVL", node->str, 5))
		node = node->next;
	eq = ft_strchr(node->str, '=') + 1;
	lvl = ft_atoi(eq) + 1;
	if (lvl > 1000)
	{
		printf("shell level (%d) too high, resetting to %d\n", lvl, lvl % 1000);
		lvl = lvl % 1000;
	}
	else if (lvl < 0)
		lvl = 0;
	shlvl = ft_itoa(lvl);
	if (update_env(env, "SHLVL", shlvl))
		return (free(shlvl), printf("caca\n"), NULL);
	return (free(shlvl), env);
}

t_envp	*create_envp_node(char *env_line, t_envp **duplicate)
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
	if (!update_shlvl(&dup_env))
		return (printf("prout\n"), NULL);
	return (dup_env);
}
