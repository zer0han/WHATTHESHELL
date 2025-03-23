/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:45:14 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/23 18:11:09 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_pipe_redir(t_exec *exec)
// {
// 	fprintf(stderr, "Pipe redirection: fd_in=%d, fd_out=%d, p_pipe=%d\n",
// 		exec->fd_in, exec->fd_out, exec->p_pipe);
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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	fprintf(stderr, "[child_process] this is before redir stdin = %d, stdout = %d\n", dup(0), dup(1));
	setup_redir(exec);
	handle_pipe_redir(exec);
	fprintf(stderr, "[child process] this is after redir stdin = %d, stdout = %d\n", dup(0), dup(1));
	if (fd_is_builtin(exec->cmd_token))
		execute_cmds(exec->cmd_token, envp, env, exec);
	else
	{
		if (!get_path(exec->cmd))
			exit(127);
		fprintf(stderr, "[child_process] Before execve: stdin = %d, stdout = %d\n", STDIN_FILENO, STDOUT_FILENO);
		fprintf(stderr, "[execve] Running %s with stdin=%d, stdout=%d\n", exec->cmd, STDIN_FILENO, STDOUT_FILENO);
		if (execve(get_path(exec->cmd), exec->args, envp) < 0)
		{
			perror("execve failed");
			exit(1);
		}
	}
	cleanup_redirection(exec->redir);
	exit(g_exit_status);
}
