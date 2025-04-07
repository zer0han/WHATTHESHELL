/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/08 00:21:29 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input(t_redir *redir, t_exec *exec)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd== -1)
		return (error_message(redir->file, errno), 0);
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
		return (error_message(redir->file, errno), 0);
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
		return (error_message(redir->file, errno), 0);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (error_message("dup2", errno), 0);
	}
	exec->fd_out = fd;
	return (1);
}

static int	handle_heredoc(t_redir *redir, t_exec *exec)
{
	int		fd;
	char	*line;
	char	*pid;
	int		i;

	i = 0;
	pid = ft_itoa(getpid());
	if (pid)
		exec->heredoc_file = ft_strjoin("/tmp/.heredoc", pid);
	if (!pid || !exec->heredoc_file)
		return (free(pid), error_message("heredoc", ENOMEM), 0);
	free(pid);
	fd = open(exec->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(exec->heredoc_file);
		return (error_message("heredoc", errno), 0);
	}
	while (1)
	{
		line = readline("> ");
		if (line && redir->delimiter[i + 1] == NULL && ft_strcmp(line, redir->delimiter[i]))
		{
			printf("i = %d\n", i);
			write (fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		else if (!line || !ft_strcmp(line, redir->delimiter[i]))
		{
			i++;
			if (redir->delimiter[i] == NULL)
				break ;
		}
	}
	close(fd);
	exec->fd_in = open(exec->heredoc_file, O_RDONLY);
	if (exec->fd_in < 0)
		return (error_message("heredoc", errno), 0);
	return (1);
}

int	apply_redirection(t_exec *exec)
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
		else if (redir->type == HEREDOC && !handle_heredoc(redir, exec))
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
	}
	if (exec->fd_out != STDOUT_FILENO)
	{
		if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(exec->fd_out);
		exec->fd_out = STDOUT_FILENO;
	}
}
