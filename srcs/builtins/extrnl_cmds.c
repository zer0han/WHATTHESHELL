/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrnl_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:31:21 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/09 01:07:23 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(t_envp *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_array(t_envp *env)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i] = ft_strdup(env->str);
		if (!array[i])
		{
			free_string_tab(array);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

static void	exec_child(char *path, char **argv, t_envp *env, t_exec *exec)
{
	close(exec->std_save[0]);
	close(exec->std_save[1]);
	execve(path, argv, env_to_array(env));
	error_message("execve", errno);
	exit(126);
}

static void	exec_parent(pid_t pid)
{
	int	status;

	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
}

void	exec_external(t_envp *env, t_exec *exec)
{
	char	*path;
	pid_t	pid;

	path = get_path(exec->cmd, env);
	if (!path)
	{
		error_message(exec->cmd, errno);
		g_exit_status = 127;
		return ;
	}
	if (exec->is_pipeline)
		exec_child(path, exec->args, env, exec);
	else
	{
		pid = fork();
		if (pid == 0)
			exec_child(path, exec->args, env, exec);
		exec_parent(pid);
	}
	free (path);
}
