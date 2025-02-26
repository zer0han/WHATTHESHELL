/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/25 19:21:11 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redir(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
		dup2(exec->fd_in, STDIN_FILENO);
	else if (exec->p_pipe != -1)
		dup2(exec->p_pipe, STDIN_FILENO);
	if (exec->fd_out != STDOUT_FILENO)
		dup2(exec->fd_out, STDOUT_FILENO);
	else if (exec->next)
		dup2(exec->fd_pipe[1], STDOUT_FILENO);
}

void	apply_redir(t_exec *exec)
{
	close(exec->fd_pipe[0]);
	if(exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
	if (exec->p_pipe != -1)
		close(exec->p_pipe);
	if (exec->next)
		close(exec->fd_pipe[1]);
}

void	child_process(t_exec *exec, t_data *code, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	handle_pipe_redir(exec);
	apply_redir(exec);
	if (exec->redir)
		redirection_process(exec->redir);
	execute_cmds(exec->cmd_token, code, envp);
	exit(0);
}