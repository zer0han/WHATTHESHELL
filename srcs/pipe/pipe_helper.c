/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/15 21:01:37 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_pipe_redir(t_exec *exec)
// {
// 	if (exec->fd_in != STDIN_FILENO)
// 		dup2(exec->fd_in, STDIN_FILENO);
// 	else if (exec->p_pipe >= 0)
// 		dup2(exec->p_pipe, STDIN_FILENO);
// 	if (exec->fd_out != STDOUT_FILENO)
// 		dup2(exec->fd_out, STDOUT_FILENO);
// 	else if (exec->next)
// 		dup2(exec->fd_pipe[1], STDOUT_FILENO);
// 	if (exec->p_pipe >= 0)
// 		close(exec->p_pipe);
// 	if (exec->next && exec->fd_pipe[1] >= 0)
// 	{
// 		close(exec->fd_pipe[1]);
// 		exec->fd_pipe[1] = -1;
// 	}
// 	if (exec->next && exec->fd_pipe[0] >= 0)
// 		close(exec->fd_pipe[0]);
// }

void	handle_pipe_redir(t_exec *exec)
{
	printf("[DEBUG] Handling pipe redirection for cmd: %s\n", exec->cmd);

	if (exec->fd_in != STDIN_FILENO)
	{
		printf("[DEBUG] Redirecting stdin to %d\n", exec->fd_in);
		if (dup2(exec->fd_in, STDIN_FILENO) == -1)
		{
			perror("[ERROR] dup2 failed for stdin");
			exit(1);
		}
		close(exec->fd_in);
	}

	if (exec->p_pipe >= 0)
	{
		printf("[DEBUG] Redirecting stdin from previous pipe %d\n", exec->p_pipe);
		if (dup2(exec->p_pipe, STDIN_FILENO) == -1)
		{
			perror("[ERROR] dup2 failed for pipe input");
			exit(1);
		}
		close(exec->p_pipe);
	}

	if (exec->fd_out != STDOUT_FILENO)
	{
		printf("[DEBUG] Redirecting stdout to %d\n", exec->fd_out);
		if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
		{
			perror("[ERROR] dup2 failed for stdout");
			exit(1);
		}
		close(exec->fd_out);
	}

	if (exec->next)
	{
		printf("[DEBUG] Redirecting stdout to pipe fd %d\n", exec->fd_pipe[1]);
		if (dup2(exec->fd_pipe[1], STDOUT_FILENO) == -1)
		{
			perror("[ERROR] dup2 failed for pipe output");
			exit(1);
		}
		close(exec->fd_pipe[1]);
	}
	printf("[DEBUG] Pipe setup: fd_in=%d, fd_out=%d, p_pipe=%d\n", exec->fd_in, exec->fd_out, exec->p_pipe);
}

void	apply_redir_pipe(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
	if (exec->p_pipe >= 0)
		close(exec->p_pipe);
	if (exec->next)
		close(exec->fd_pipe[1]);
}

void	child_process(t_exec *exec, char **envp)
{
// 	signal(SIGQUIT, SIG_DFL);
// 	handle_pipe_redir(exec);
// 	setup_redir(exec);
// 	if (exec->redir)
// 		redirection_process(exec->redir);
// 	execute_cmds(exec->cmd_token, envp, exec);
// 	exit(0);
	signal(SIGQUIT, SIG_DFL);
	setup_redir(exec);
	handle_pipe_redir(exec);
	execve(exec->cmd, exec->args, envp);
	perror("execve");
	exit(1);
}
