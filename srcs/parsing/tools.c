/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:53:43 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/28 17:44:20 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_path(char *cmd)
{
	char	*env_var;
	char	*folder;
	char	*path;
	char	**cmd_paths;
	int		i;

	env_var = getenv("PATH");
	cmd_paths = ft_split(env_var, ':');
	i = 0;
	while (cmd_paths[i] && cmd)
	{
		folder = ft_fstrjoin(cmd_paths[i], "/");
		path = ft_fstrjoin(folder, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (free_tab(&cmd_paths[i + 1], 0), path);
		free(path);
		i++;
	}
	free(cmd_paths);
	return (NULL);
}

void	free_tab(char **tab, int code)
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

char	*ft_fstrjoin(char *s1, char *s2)
{
	char	*dst;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	dst = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dst == NULL)
		return (free(s1), NULL);
	while (s1 != NULL && s1[i])
		dst[j++] = s1[i++];
	i = 0;
	free (s1);
	while (s2 != NULL && s2[i])
		dst[j++] = s2[i++];
	dst[j] = '\0';
	return (dst);
}

int	is_builtin(char *cmd)
{
	int	len;

	len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "cd", len) || !ft_strncmp(cmd, "export", len) \
	|| !ft_strncmp(cmd, "unset", len) || !ft_strncmp(cmd, "exit", len))
		return (1);
	return (0);
}
