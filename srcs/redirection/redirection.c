/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 18:48:11 by rdalal           ###   ########.fr       */
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

// static int  handle_input(t_redir *redir, t_exec *exec)
// {
//     int fd;

//     if (exec->fd_in != STDIN_FILENO) // Close previous input FD
//         close(exec->fd_in);
//     fd = open(redir->file, O_RDONLY);
//     if (fd == -1)
// 	{
// 		close(fd);
//         return (error_message(redir->file, errno), 0);
// 	}
//     exec->fd_in = fd;
//     return (1);
// }

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

// Modified handle_output
// static int  handle_output(t_redir *redir, t_exec *exec)
// {
//     int fd;

//     if (exec->fd_out != STDOUT_FILENO) // Close previous output FD
//         close(exec->fd_out);
//     fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
// 	{
// 		close(fd);
//         return (error_message(redir->file, errno), 0);
// 	}
//     exec->fd_out = fd;
//     return (1);
// }

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
	exec->fd_in = fd;
	return (1);
}

static char	*handle_heredoc(t_redir *redir, t_exec *exec)
{
	int		fd;
	char	*line;
	char	*temp_file;

	temp_file = ft_strjoin("/tmp/.heredoc", ft_itoa(getpid()));
	if (!temp_file)
		return (error_message("heredoc", ENOMEM), NULL);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(temp_file);
		return (error_message("heredoc", errno), NULL);
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
	exec->fd_in = fd;
	close(fd);
	// unlink(temp_file);
	// free(temp_file);
	// if (fd < 0)
	// 	return (error_message("heredoc", errno), NULL);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	return (temp_file);
}

// void	print_redir_list(t_redir *redir)
// {
// 	int	i;

// 	i = 0;
// 	while (redir)
// 	{
// 		fprintf(stderr, "REDIR NODE %d\n", i);
// 		fprintf(stderr, "type : %d | file : %s\n\n", (int)redir->type, redir->file);
// 		i++;
// 		redir = redir->next;
// 	}
// }

// int	apply_redirection(t_exec *exec)
// {
// 	t_redir	*redir;
// 	int		pass;

// 	redir = exec->redir;
// 	while (redir)
// 	{
// 		pass = 0;
// 		if (redir->type == REDIR_OUT)
// 			pass = handle_output(redir, exec);
// 		else if (redir->type == REDIR_APPEND)
// 			pass = handle_append(redir, exec);
// 		else if (redir->type == REDIR_IN)
// 			pass = handle_input(redir, exec);
// 		else if (redir->type == HEREDOC)
// 			pass = handle_heredoc(redir, exec);
// 		if (!pass)
// 		{
			
// 		}
// 	}
// }

// int apply_redirection(t_exec *exec)
// {
//     t_redir *redir = exec->redir;

// 	// print_redir_list(redir);
//     while (redir) {
//         int success = 0;
//         if (redir->type == REDIR_OUT)
//             success = handle_output(redir, exec);
// 		else if (redir->type == REDIR_APPEND)
// 			success = handle_append(redir, exec);
//         else if (redir->type == REDIR_IN)
//             success = handle_input(redir, exec);
//         else if (redir->type == HEREDOC)
//             success = handle_heredoc(redir, exec);
//         if (!success) {
//             // Cleanup all opened FDs
//             if (exec->fd_in != STDIN_FILENO) close(exec->fd_in);
//             if (exec->fd_out != STDOUT_FILENO) close(exec->fd_out);
//             return (0);
//         }
//         redir = redir->next;
//     }
//     return (1);
// }

int	apply_redirection(t_exec *exec)
{
	t_redir	*redir;

	redir = exec->redir;
	while (redir)
	{
		if (redir->type == REDIR_OUT && !handle_output(redir, exec))
			return (fprintf(stderr, "[apply_redir] set output fd: %d\n", exec->fd_out), 0);
		else if (redir->type == REDIR_APPEND && !handle_append(redir, exec))
		return (fprintf(stderr, "[apply_redir] set output fd: %d\n", exec->fd_out), 0);
		else if (redir->type == REDIR_IN && !handle_input(redir, exec))
			return (fprintf(stderr, "[apply_redir] set input fd: %d\n", exec->fd_in), 0);
		else if (redir->type == HEREDOC && !handle_heredoc(redir, exec))
			return (0);
		redir = redir->next;
	}
	return (1);
}

void	setup_redir(t_exec *exec)
{
	fprintf(stderr, "before dup2 : fd_in = %d, fd_out = %d\n", exec->fd_in, exec->fd_out);
	if (exec->fd_in != STDIN_FILENO)// && exec->fd_in != -1)
	{
		fprintf(stderr, "[setup_redir] redirecting input from fd %d to STDIN\n", exec->fd_in);
		if (dup2(exec->fd_in, STDIN_FILENO) == -1)
		{
			perror("Error: dup2 failed in stdin");
			exit(EXIT_FAILURE);
		}
		close(exec->fd_in);
		exec->fd_in = STDIN_FILENO;
	}
	if (exec->fd_out != STDOUT_FILENO)// && exec->fd_out != -1)
	{
		fprintf(stderr, "[setup_redir] redirecting output from fd %d to STDOUT\n", exec->fd_out);
		if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
		{
			perror("Error: dup2 failed in stdout");
			exit(EXIT_FAILURE);
		}
		close(exec->fd_out);
		exec->fd_out = STDOUT_FILENO;
	}
	fprintf(stderr, "after dup2: fd_in = %d, fd_out = %d\n", exec->fd_in, exec->fd_out);
}
