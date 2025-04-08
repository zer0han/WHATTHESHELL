/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:53:43 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:09:10 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_tab(char **tab, int code)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
	if (code != 0)
		free(tab);
}

char	*get_path(char *cmd, t_envp *env)
{
	char	*env_var;
	char	*folder;
	char	*path;
	char	**cmd_paths;
	int		i;

	if (!access(cmd, F_OK | X_OK))
		return (ft_strdup(cmd));
	env_var = my_getenv("PATH", &env);
	if (!env_var)
		return (NULL);
	cmd_paths = ft_split(env_var, ':');
	i = 0;
	while (cmd_paths[i] && cmd)
	{
		folder = ft_fstrjoin(cmd_paths[i], "/");
		path = ft_fstrjoin(folder, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (free_tab(&cmd_paths[i + 1], 0), \
					free(cmd_paths), path);
		free(path);
		i++;
	}
	free(cmd_paths);
	return (NULL);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	is_space(char s)
{
	if (s == ' ' || (s >= '\t' && s <= '\r'))
		return (1);
	return (0);
}

void	alloc_fail(char **result, int iword)
{
	int	i;

	i = 0;
	while (i < iword)
	{
		if (result[i])
			free (result[i]);
		i++;
	}
	free (result);
}

// int	is_builtin(char *cmd)
// {
// 	int	len;

// 	len = ft_strlen(cmd);
// 	if (!ft_strncmp(cmd, "cd", len) || !ft_strncmp(cmd, "export", len) /*\*/
// 	|| !ft_strncmp(cmd, "unset", len) || !ft_strncmp(cmd, "exit", len))
// 		return (1);
// 	return (0);
// }
