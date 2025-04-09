/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/09 22:01:43 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redir(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO && !exec->redir_applied[0])
		dup2(exec->fd_in, STDIN_FILENO);
	else if (exec->p_pipe >= 0 && !exec->redir_applied[0])
		dup2(exec->p_pipe, STDIN_FILENO);
	if (exec->fd_out != STDOUT_FILENO && !exec->redir_applied[1])
	{
		dup2(exec->fd_out, STDOUT_FILENO);
		close (exec->fd_out);
	}
	else if (exec->next && !exec->redir_applied[1])
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
	char	*path;

	if (apply_redirection(exec, env))
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
	path = get_path(exec->cmd, env);
	if (!path)
		exit (127);
	execve(path, exec->args, env_to_array(env));
	perror("execve");
	free(path);
	free_array(env_to_array(env));
	exit (126);
}
