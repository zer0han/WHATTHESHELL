/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:44:07 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/16 18:14:47 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

/****functions needed*/
/* one for creating a pipe
 * one for handling child processes
 * one for executing cmds in child processes
 * one for checking if a cmd is an absolute path like (e.g /bin/ls)
 ** if it contains '/' it is an absolute path so we don't need path to lookup
* */

void	setup_child_process(t_exec *exec, char **envp)
{
	pid_t	pid;
	
	if (exec->next && pipe(exec->fd_pipe) == -1)
		return (perror("pipe failed"));
	pid = fork();
	if (pid == 0)
	{
		if (exec->redir)
			setup_redir(exec);
		handle_pipe_redir(exec);
		if (!get_path(exec->cmd))
			exit (127);
		execve(get_path(exec->cmd), exec->args, envp);
		perror("execve failed");
		exit(1);
	}
	else if (pid > 0)
	{
		exec->pid = pid;
		if (exec->p_pipe >= 0)
			close(exec->p_pipe);
		if (exec->next)
		{
			close(exec->fd_pipe[1]);
			exec->next->p_pipe = exec->fd_pipe[0];
		}
	}
	else
		perror("fork");
}

void	wait_for_children(t_exec *exec)
{
	int	status;

	status = 0;
	while (exec)
	{
		waitpid(exec->pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		exec = exec->next;
	}
}

void	exec_pipeline(t_exec *exec, char **envp)
{
	t_exec	*head;

	head = exec;
	while (exec)
	{
		if (exec->redir)
			redirection_process(exec, exec->redir);
		setup_child_process(exec, envp);
		// if (exec->p_pipe >= 0)
		// 	close(exec->p_pipe);
		exec = exec->next;
	}
	wait_for_children(head);
	while (head)
	{
		if (head->fd_pipe[0] != -1)
			close(head->fd_pipe[0]);
		if (head->fd_pipe[1] != -1)
			close (head->fd_pipe[1]);
		head = head->next;
	}
}

/*void	setup_pipe(t_exec *exec, int new_pipe[2])
{
	if (pipe(new_pipe) == -1)
	{
		perror("whattheshell: pipe");
		exit(EXIT_FAILURE);
	}
	exec->fd_out = new_pipe[1];
}

void	connect_pipes(t_exec *exec, int prev_pipe, int new_pipe[2])
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (exec->fd_out != STDOUT_FILENO)
	{
		dup2(exec->fd_out, STDOUT_FILENO);
		close(exec->fd_out);
	}
	else if (exec->next)
	{
		dup2(new_pipe[1], STDOUT_FILENO);
		close(new_pipe[1]);
	}
	close(new_pipe[0]);
}

void	child_process(t_exec *exec, int prev_pipe, int new_pipe[2], char **envp)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	connect_pipes(exec, prev_pipe, new_pipe);
	if (exec->redir)
		redirection_process(exec->redir);
	if (exec->cmd)
		execute_cmds(exec->cmd_token, exec->arg_data, envp);
	exit (0);
}

void	exec_pipeline(t_exec *exec, char **envp)
{
	int		new_pipe[2];
	int		prev_pipe;
	pid_t	pid;

	prev_pipe = -1;
	while (exec)
	{
		if (exec->next)
			setup_pipe(exec, new_pipe);
		pid = fork();
		if (pid == 0)
			child_process(exec, prev_pipe, new_pipe, envp);
		else if (pid > 0)
			exec->pid = pid;
		else
			handle_error("fork", errno, &exec->cmd_token);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (exec->next)
			prev_pipe = new_pipe[0];
		else
			close(new_pipe[0]);
		close (exec->fd_out);
		exec = exec->next;
	}
}*/
 