/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:21:49 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 13:25:58 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_heredoc_file(char **temp)
{
	int	fd;

	*temp = ft_strdup("/tmp/....");
	fd = mkstemp(*temp);
	if (fd == -1)
		return (error_message("heredoc", errno), -1);
	return (fd);
}

void	read_heredoc_content(int fd, char *limit)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limit) == 0)
		{
			free (line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_token *redir, t_token *file)
{
	char	*temp;
	int		fd;

	(void)redir;
	fd = create_heredoc_file(&temp);
	if (fd == -1)
		return (1);
	read_heredoc_content(fd, file->input);
	close(fd);
	fd = open(temp, O_RDONLY);
	unlink(temp);
	free(temp);
	if (fd == -1)
		return (error_message("heredoc", errno), 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
