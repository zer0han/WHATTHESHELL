/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:44:07 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/01 18:10:16 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	setup_child_process(t_exec *exec, t_envp *env)
{
	pid_t	pid;

	if (exec->next && pipe(exec->fd_pipe) == -1)
		return (perror("pipe failed"));
	pid = fork();
	if (pid == 0)
	{
		child_process(exec, env);
		exit(0);
	}
	else
	{
		close(exec->std_save[0]);
		close(exec->std_save[1]);
		exec->pid = pid;
		if (exec->p_pipe != -1)
			close(exec->p_pipe);
		if (exec->next)
		{
			close(exec->fd_pipe[1]);
			exec->next->p_pipe = exec->fd_pipe[0];
		}
	}
}

void	wait_for_children(t_exec *exec)
{
	int	status;

	while (exec)
	{
		if (exec->pid != -1)
		{
			waitpid(exec->pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
		exec = exec->next;
	}
}

void	exec_pipeline(t_exec *exec, t_envp *env)
{
	t_exec	*head;

	head = exec;
	while (exec && exec->next)
	{
		setup_child_process(exec, env);
		close(exec->fd_pipe[1]);
		exec = exec->next;
	}
	if (exec)
		setup_child_process(exec, env);
	wait_for_children(head);
}
