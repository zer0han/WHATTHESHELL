/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_hanlder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:27:39 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/25 15:29:19 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_output(t_token *redir, t_token *file, t_token **token)
{
	int	fd;

	fd = open (file->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (handle_error(file->input, errno, token), 1);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

int	handle_append(t_token *redir, t_token *file, t_token **token)
{
	int	fd;

	fd = open(file->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (handle_error(file->input, errno, token), 1);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

int	handle_input(t_token *redir, t_token *file, t_token **token)
{
	int	fd;

	fd = open(file->input, O_RDONLY);
	if (fd == -1)
		return (handle_error(file->input, errno, token), 1);
	dup2(fd, STDIN_FILENO);
	close (fd);
	return (0);
}
