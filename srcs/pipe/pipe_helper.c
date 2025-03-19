/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/19 17:52:23 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redir(t_exec *exec)
{
	fprintf(stderr, "Pipe redirection: fd_in=%d, fd_out=%d, p_pipe=%d\n",
		exec->fd_in, exec->fd_out, exec->p_pipe);
	if (exec->fd_in != STDIN_FILENO)
		dup2(exec->fd_in, STDIN_FILENO);
	else if (exec->p_pipe >= 0)
		dup2(exec->p_pipe, STDIN_FILENO);
	if (exec->fd_out != STDOUT_FILENO)
		dup2(exec->fd_out, STDOUT_FILENO);
	else if (exec->next)
		dup2(exec->fd_pipe[1], STDOUT_FILENO);
	if (exec->p_pipe >= 0)
		close(exec->p_pipe);
	if (exec->next && exec->fd_pipe[1] >= 0)
	{
		close(exec->fd_pipe[1]);
		exec->fd_pipe[1] = -1;
	}
	if (exec->next && exec->fd_pipe[0] >= 0)
		close(exec->fd_pipe[0]);
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

void	child_process(t_exec *exec, char **envp, t_envp *env)
{
	signal(SIGQUIT, SIG_DFL);
	setup_redir(exec);
	handle_pipe_redir(exec);
	execute_cmds(exec->cmd_token, envp, env, exec);
	cleanup_redirection(exec->redir);
	exit(g_exit_status);
	// signal(SIGQUIT, SIG_DFL);
	// handle_pipe_redir(exec);
	// execve(exec->cmd, exec->args, envp);
	// perror("execve");
	// exit(1);
}
