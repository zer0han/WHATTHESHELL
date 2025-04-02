/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/01 18:07:38 by rdalal           ###   ########.fr       */
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
	{
		dup2(exec->fd_out, STDOUT_FILENO);
		close (exec->fd_out);
	}
	else if (exec->next)
	{
		dup2(exec->fd_pipe[1], STDOUT_FILENO);
		close(exec->fd_pipe[1]);
	}
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

void	child_process(t_exec *exec, t_envp *env)
{
	if (apply_redirection(exec))
		setup_redir(exec);
	handle_pipe_redir(exec);
	if (exec->p_pipe != -1)
		close(exec->p_pipe);
	if (exec->fd_pipe[0] != -1)
		close(exec->fd_pipe[0]);
	if (exec->fd_pipe[1] != -1)
		close(exec->fd_pipe[1]);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
	if (fd_is_builtin(exec->cmd_token) && !exec->is_pipeline)
		dispatch_cmds(exec->cmd_token, env, exec);
	if (!get_path(exec->cmd))
		exit (127);
	execve(get_path(exec->cmd), exec->args, env_to_array(env));
	perror("WHATTHESHELL: execve");
	free_array(env_to_array(env));
	exit (126);
}
