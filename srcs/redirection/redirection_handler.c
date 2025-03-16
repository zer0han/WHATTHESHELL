/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:27:39 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/15 19:24:13 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_output(t_token *redir, t_token *file)//, t_token *token)
{
	int	fd;

	(void)redir;
	//(void)token;
	fd = open (file->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

int	handle_append(t_token *redir, t_token *file)//, t_token *token)
{
	int	fd;

	(void)redir;
	//(void)token;
	fd = open(file->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

// int	handle_input(t_token *redir, t_token *file, t_token *token)
// {
// 	int	fd;

// 	(void)redir;
// 	(void)token;
// 	fd = open(file->input, O_RDONLY);
// 	if (fd == -1)
// 		return (1);
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("ERROR dup2 failed for input");
// 		close(fd);
// 		return (1);
// 	}
// 	close (fd);
// 	return (0);
// }

int	handle_input(t_token *file)
{
	int	fd;

	// Open the input file
	fd = open(file->input, O_RDONLY);
	if (fd == -1)
	{
		perror("ERROR open failed for input file");
		return (1); // Return error code
	}

	// Redirect stdin to the file descriptor
	printf("DEBUGGGG REDIR STDIN FROM FILE: %s (fd: %d)\n", file->input, fd);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("ERROR dup2 failed for input");
		close(fd); // Close the file descriptor on error
		return (1); // Return error code
	}

	// Close the file descriptor after redirection
	printf("DEBUG SUCCESSFULLY REDIR STDIN FROM FD %d\n", fd);
	close(fd);
	return (0); // Success
}
