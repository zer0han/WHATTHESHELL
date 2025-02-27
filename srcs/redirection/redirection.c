/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 13:27:17 by rdalal           ###   ########.fr       */
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

int	apply_redirection(t_token *redir, t_token *file, t_token **token)
{
	if (ft_strcmp(redir->input, ">") == 0)
		return (handle_output(redir, file, token));
	if (ft_strcmp(redir->input, ">>") == 0)
		return (handle_append(redir, file, token));
	if (ft_strcmp(redir->input, "<") == 0)
		return (handle_input (redir, file, token));
	if (ft_strcmp(redir->input, "<<") == 0)
		return (handle_heredoc(redir, file));
	return (0);
}

void	redirection_process(t_token *token)
{
	t_token	*file;
	t_token	*current;

	current = token;
	while (token)
	{
		if (current->input && (ft_strcmp(current->input, ">") == 0 \
			|| ft_strcmp(current->input, ">>") == 0 \
			|| ft_strcmp(current->input, "<") == 0 \
			|| ft_strcmp(current->input, "<<") == 0))
		{
			file = current->right;
			if (!file || !file->input)
				handle_error("syntax error", EINVAL, &token);
			current = file->right;
		}
		else
			current = current->right;
	}
}

/*void	heredoc_redirection(t_token *hd_token)
{
	char	*delimiter;
	char	*line;
	char	*temp_name;
	int		fd_temp;

	temp_name = "file_name"; //use a unique file in minishell to check the heredoc redirection
	if (!hd_token || !hd_token->right || !hd_token->right->input)
	{
		ft_putstr_fd("Heredoc: missing delimiter\n", STDERR_FILENO);
		exit (1);
	}
	delimiter = hd_token->right->input;
	fd_temp = open(temp_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_temp == -1)
	{
		perror("open temp file here");
		exit (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free (line);
			break ;
		}
		write(fd_temp, line, ft_strlen(line));
		write(fd_temp, "\n", 1);
		free (line);
	}
	close (fd_temp);
	fd_temp = open (temp_name, O_RDONLY);
	if (fd_temp == -1)
	{
		perror("open temp file for heredoc reading\n");
		exit (1);
	}
	dup2(fd_temp, STDIN_FILENO);
	close (fd_temp);
}

void	redirection_process(t_token *tokens)
{
	t_token	*current;
	int		fd;

	current = tokens;
	while (current)
	{
		if (current->type && ft_strcmp(current->type, "redirection") == 0)
		{
			if (ft_strcmp(current->input, ">") == 0)
			{
				//output redirection: turncate (or create)
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd == -1)
					{
						perror("open");
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					closee (fd);
				}
			}
			else if (ft_strcmp(current->input, ">>") == 0)
			{
			// append output redirection
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
					{
						perror("open");
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
			}
			else if (ft_strcmp(current->input, "<") == 0)
			{
				//input redirection: open file for reading
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_RDONLY);
					if (fd == -1)
					{
						perror("open");
						exit (1);
					}
					dup2(fd, STDIN_FILENO);
					close (fd);
				}
			}
			else if (ft_strcmp(current->input, "<<") == 0)
				heredoc_redirection(current);
		}
		current = current->right;
	}
}*/

