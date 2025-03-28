/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/27 20:07:49 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_pipe_redir(t_exec *exec)
// {
// 	fprintf(stderr, "Pipe redirection: fd_in=%d, fd_out=%d, p_pipe=%d\n",
// 		exec->fd_in, exec->fd_out, exec->p_pipe);
// 	if (exec->p_pipe >= 0 && exec->fd_in == STDIN_FILENO)
// 	{
// 		fprintf(stderr, "[handle_pipe_redir] Setting up input from previous pipe %d\n", exec->p_pipe);
// 		dup2(exec->p_pipe, STDIN_FILENO);
// 		close(exec->p_pipe);
// 	}
// 	if (exec->fd_out == STDOUT_FILENO)
// 	{
// 		fprintf(stderr, "[handle_pipe_redir] setting up output to fd %d\n", exec->fd_out);
// 		dup2(exec->fd_out, STDOUT_FILENO);
// 		close(exec->fd_out);
// 	}
// 	else if (exec->next)
// 	{
// 		fprintf(stderr, "[handle_pipe_redir] setting up output to pipe %d\n", exec->fd_pipe[1]);
// 		dup2(exec->fd_pipe[1], STDOUT_FILENO);
// 		close(exec->fd_pipe[1]);
// 	}
// 	if (exec->next && exec->fd_pipe[0] >= 0)
// 		close(exec->fd_pipe[0]);
// }


// void	apply_redir_pipe(t_exec *exec)
// {
// 	if (exec->fd_in != STDIN_FILENO)
// 		close(exec->fd_in);
// 	if (exec->fd_out != STDOUT_FILENO)
// 		close(exec->fd_out);
// 	if (exec->p_pipe >= 0)
// 		close(exec->p_pipe);
// 	if (exec->next)
// 		close(exec->fd_pipe[1]);
// }

// void	child_process(t_exec *exec, char **envp, t_envp *env)
// {
// 	(void)env;
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	fprintf(stderr, "[child_process] this is before redir stdin = %d, stdout = %d\n", dup(0), dup(1));
// 	setup_redir(exec);
// 	handle_pipe_redir(exec);
// 	fprintf(stderr, "[child process] this is after redir stdin = %d, stdout = %d\n", dup(0), dup(1));
// 	if (fd_is_builtin(exec->cmd_token))
// 		execute_cmds(exec->cmd_token, envp, env, exec);
// 	if (!get_path(exec->cmd))
// 		exit(127);
// 	fprintf(stderr, "[child_process] Before execve: stdin = %d, stdout = %d\n", STDIN_FILENO, STDOUT_FILENO);
// 	fprintf(stderr, "[execve] Running %s with stdin=%d, stdout=%d\n", exec->cmd, STDIN_FILENO, STDOUT_FILENO);
// 	if (execve(get_path(exec->cmd), exec->args, envp) < 0)
// 	{
// 		perror("execve failed");
// 		exit(1);
// 	}
// 	cleanup_redirection(exec->redir);
// 	exit(g_exit_status);
// }

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
	// if (exec->next && exec->fd_pipe[1] >= 0)
	// {
	// 	close(exec->fd_pipe[1]);
	// 	exec->fd_pipe[1] = -1;
	// }
	if (exec->next && exec->fd_pipe[0] >= 0)
		close(exec->fd_pipe[0]);
}

void		child_process(t_exec *exec, t_envp *env)
{
	if (apply_redirection(exec))
		setup_redir(exec);
	handle_pipe_redir(exec);
	
	// Close all inherited FDs not used
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
