/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/11 21:50:06 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redir(t_exec *exec)
{
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

void	apply_redir(t_exec *exec)
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
	handle_pipe_redir(exec);
	setup_redir(exec);
	execve(exec->cmd, exec->args, envp);
	perror("execve");
	exit(1);
}
