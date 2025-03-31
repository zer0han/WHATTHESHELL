/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrnl_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:31:21 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 17:59:56 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_envp *env)
{
	t_envp	*temp;
	char	**array;
	int		count;
	int		i;

	count = 0;
	temp = env;
	while (temp && ++count)
		temp = temp->next;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	temp = env;
	while (temp)
	{
		array[i] = ft_strdup(temp->str);
		if (!array[i])
		{
			while (--i >= 0)
				free (array[i]);
			free (array);
			return (NULL);
		}
		i++;
		temp = temp->next;
	}
	array[i] = NULL;
	return (array);
}

void	exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec)
{
	char	*path;
	char	**argv;
	pid_t	pid;
	int		status;

	(void)tokens;
	(void)envp;
	path = get_path(exec->cmd);
	if (!path)
	{
		error_message(exec->cmd, errno);
		g_exit_status = 127;
		return ;
	}
	argv = exec->args;
	if (exec->is_pipeline)
	{
		execve(path, argv, env_to_array(env));
		error_message("execve", errno);
		exit(126);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			execve(path, argv, env_to_array(env));
			error_message("execve", errno);
			exit(126);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			g_exit_status = WEXITSTATUS(status);
		}
	}
	free (path);
}
