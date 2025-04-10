/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/10 19:49:54 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input(t_redir *redir, t_exec *exec)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		return (error_message(redir->file, errno), g_exit_status = 1, 0);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	exec->fd_in = fd;
	return (1);
}

static int	handle_output(t_redir *redir, t_exec *exec)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (error_message(redir->file, errno), g_exit_status = 1, 0);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
	exec->fd_out = fd;
	return (1);
}

static int	handle_append(t_redir *redir, t_exec *exec)
{
	int	fd;

	(void)exec;
	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (error_message(redir->file, errno), g_exit_status = 1, 0);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
	exec->fd_out = fd;
	return (1);
}

int	apply_redirection(t_exec *exec, t_envp *env)
{
	t_redir	*redir;

	redir = exec->redir;
	while (redir)
	{
		if (redir->type == REDIR_OUT && !handle_output(redir, exec))
			return (0);
		else if (redir->type == REDIR_APPEND && !handle_append(redir, exec))
			return (0);
		else if (redir->type == REDIR_IN && !handle_input(redir, exec))
			return (0);
		else if (redir->type == HEREDOC && !handle_heredoc(redir, exec, env))
			return (0);
		redir = redir->next;
	}
	return (1);
}

void	setup_redir(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
	{
		if (dup2(exec->fd_in, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(exec->fd_in);
		exec->fd_in = STDIN_FILENO;
		exec->redir_applied[0] = 1;
	}
	if (exec->fd_out != STDOUT_FILENO)
	{
		if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(exec->fd_out);
		exec->fd_out = STDOUT_FILENO;
		exec->redir_applied[1] = 1;
	}
}
