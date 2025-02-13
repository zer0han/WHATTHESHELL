/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:44:07 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/13 15:46:38 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/****functions needed*/
/* one for creating a pipe
 * one for handling child processes
 * one for executing cmds in child processes
 * one for checking if a cmd is an absolute path like (e.g /bin/ls)
 ** if it contains '/' it is an absolute path so we don't need path to lookup
* */

int	make_pipe(int m_pipe[2])
{
	if (pipe(m_pipe) == -1)
	{
		perror("pipe failed");
		return (1);
	}
	return (0);
}

void	child_process1(t_exec *exec, int prev_pipe, char ***envp)
{
	int	m_pipe[2];

	m_pipe[0] = -1;
	m_pipe[1] = -1;
	if (exec->next)
		make_pipe(m_pipe);
	signal(SIGINT, SIG_IGN);
	exec->pid = fork();
	if (exec->pid == -1)
		return (perror("fork"), exit(EXIT_FAILURE));
	else if (exec->pid == 0)
		child_process2(exec, prev_pipe, m_pipe, envp);
	else
	{
		if (prev_pipe != -1)
			close(prev_pipe);
		if (exec->next)
			close(m_pipe[1]);
	}
}

void	child_process2(t_exec *exec, int prev_pipe, int m_pipe[2], char ***envp)
{
	int	exit_code;
	
	exit_code = 0;
	signal(SIGQUIT, SIG_DFL);
	if (exec->fd_in != STDIN_FILENO)
	{
		dup2(exec->fd_in, STDIN_FILENO);
		close (exec->fd_in);
	}
	else if (prev_pipe != -1)
		dup2(prev_pipe, STDERR_FILENO);
	if (exec->fd_out != STDOUT_FILENO)
	{
		dup2(exec->fd_out, STDOUT_FILENO);
		close (exec->fd_out);
	}
	else if (exec->next)
		dup2(m_pipe[1], STDOUT_FILENO);
	close(m_pipe[0]);
	close(m_pipe[1]);
	if (prev_pipe != -1)
		close(prev_pipe);
	dispatch_cmds(exec->redir, exec->args, envp);
	exit (0);
}

void	exec_pipeline(t_exec *exec, char ***envp)
{
	int		prev_pipe;
	t_exec	*exec_ptr;

	exec_ptr = exec;
	prev_pipe = -1;
	while (exec)
	{
		child_process1(exec, prev_pipe, envp);
		if (prev_pipe != -1)
			close(prev_pipe);
		prev_pipe = exec->fd_pipe[0];
		exec = exec->next;
	}
	while (exec)
	{
		waitpid(exec_ptr->pid, NULL, 0);
		exec = exec_ptr->next;
	}
}
