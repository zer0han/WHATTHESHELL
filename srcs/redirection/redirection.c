/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/23 23:53:41 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* in parsing created a linked list of t_token
* for redirection tokens:
** token->input is one: '>' '<' '>>' '<<'
** token->type is redirection
** token->right is a pointer to another t_token holding the filename (in its input feild)
*/

/*
* Heredoc redirection: this is more complex
* a full implementation would read lines until a delimiter is found
* for simplicity, we will just print a message
* in the finished implimentation
** create a temp file
** read from the stdin until the delimiter is found
** then open that file for reading and dup2 it to STDIN
* expects heredoc_token->right->input to have the delimiter.
* reads input lines from the user until the delimiter is reached
* does not add lines to history
* writes the content to a temp file
* redirects STDIN to the temp file
*/

// static int	handle_output(t_redir *redir, t_exec *exec)
// {
// 	int	fd;

// 	(void) exec;
// 	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 		return (error_message(redir->file, errno), 0);
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{	close(fd);
// 		return (error_message("dup2", errno), 0);
// 	}
// 	close (fd);
// 	return (1);
// }

// static int  handle_input(t_redir *redir, t_exec *exec)
// {
// 	int	fd;

// 	(void)exec;
// 	fd = open(redir->file, O_RDONLY);
// 	if (fd== -1)
// 		return (error_message(redir->file, errno), 0);
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{	close(fd);
// 		return (error_message("dup2", errno), 0);
// 	}
// 	close (fd);
// 	return (1);
// }


static int  handle_output(t_redir *redir, t_exec *exec)
{
	int	fd;

	(void)exec;
	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd== -1)
		return (error_message(redir->file, errno), 0);
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
	{	close(fd);
		return (error_message("dup2", errno), 0);
	}
	close (fd);
	return (1);
}

static int  handle_input(t_redir *redir, t_exec *exec)
{
	int	fd;

	(void)exec;
	fd = open(redir->file, O_RDONLY);
	if (fd== -1)
		return (error_message(redir->file, errno), 0);
	exec->fd_in = fd;
	return (1);
}

static int	handle_heredoc(t_redir *redir, t_exec *exec)
{
	int		fd;
	char	*line;
	char	*temp_file;

	temp_file = ft_strjoin("/tmp/.heredoc", ft_itoa(getpid()));
	if (!temp_file)
		return (error_message("heredoc", ENOMEM), 0);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(temp_file);
		return (error_message("heredoc", errno), 0);
	}
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->delimiter) == 0)
		{
			free (line);
			break ;
		}
		write (fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close (fd);
	exec->fd_in = open(temp_file, O_RDONLY);
	unlink(temp_file);
	free(temp_file);
	if (fd < 0)
		return (error_message("heredoc", errno), 0);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
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
	if (exec->redir)
		apply_redirection(exec);
	fprintf(stderr, "before dup2 : fd_in = %d, fd_out = %d\n", exec->fd_in, exec->fd_out);
	if (exec->fd_in != STDIN_FILENO && exec->fd_in != -1)
	{
        fprintf(stderr, "[setup_redir] redirecting input from fd %d to STDIN\n", exec->fd_in);
		// if (dup2(exec->fd_in, STDIN_FILENO) == -1)
		// {
		// 	perror("Error: dup2 failed in stdin");
		// 	exit(EXIT_FAILURE);
		// }
		// close (exec->fd_in);
		dup2(exec->fd_in, STDIN_FILENO);
		close(exec->fd_in);
		exec->fd_in = STDIN_FILENO;
	}
	if (exec->fd_out != STDOUT_FILENO && exec->fd_out != -1)
	{
        fprintf(stderr, "[setup_redir] redirecting output from fd %d to STDOUT\n", exec->fd_in);
		// if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
		// {
		// 	perror("Error: dup2 failed in stdout");
		// 	exit(EXIT_FAILURE);
		// }
		// close (exec->fd_out);
		dup2(exec->fd_out, STDOUT_FILENO);
		close(exec->fd_out);
		exec->fd_out = STDOUT_FILENO;
	}
	fprintf(stderr, "after dup2: fd_in = %d, fd_out = %d\n", exec->fd_in, exec->fd_out);
}


