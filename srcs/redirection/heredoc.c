/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:21:49 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/26 15:28:19 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**setup_cmd(t_token *node)
{
	char	**cmd;
	int		i;

	i = 0;
	while (node && \
	(!ft_strcmp(node->type, "arg") || !ft_strcmp(node->type, "option")))
	{
		i++;
		node = node->right;
	}
	cmd = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node && \
	(!ft_strcmp(node->type, "arg") || !ft_strcmp(node->type, "option")))
	{
		cmd[i] = ft_strdup(node->input);
		node = node->right;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

static int	create_temp_file(void)
{
	int		fd;
	char	*temp_file;

	temp_file = ft_strdup("/tmp/.heredoc");
	if (!temp_file)
		return (-1);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free (temp_file);
	if (fd < 0)
		return (-1);
	return (fd);
}

static int	redirect_heredoc(int fd)
{
	
}

static int	exec_cmds(char *cmd, char **args)
{
	if (fd_is_builtin())
}

int	heredoc(t_token **tokens)
{
	t_token	*node;
	char	*delim; // a free
	char	**cmd; // a free
	char	*line;
	int		fd;

	signal(SIGINT, SIG_DFL);
	node = *tokens;
	cmd = setup_cmd(node);
	while (node && ft_strcmp(node->type, "delimiter"))
		node = node->right;
	delim = ft_strdup(node->input);
	fd = create_temp_file();
	if (fd < 0)
		return (free(delim), free_string_tab(cmd), EXIT_FAILURE);
	line = readline("> ");
	while (line && !ft_strncmp(line, delim, ft_strlen(line)))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (free(delim), free_string_tab(cmd), EXIT_FAILURE);
	return (exec_cmds(*cmd, cmd + 1))
}

// static int	handle_heredoc(t_redir *redir, t_exec *exec)
// {
// 	int		fd;
// 	char	*line;
// 	char	*temp_file;

//	temp_file = ft_strjoin("/tmp/.heredoc", ft_itoa(getpid()));
// 	if (!temp_file)
// 		return (error_message("heredoc", ENOMEM), 0);
// 	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		free(temp_file);
// 		return (error_message("heredoc", errno), 0);
// 	}
// 	signal(SIGINT, SIG_DFL);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, redir->delimiter) == 0)
// 		{
// 			free (line);
// 			break ;
// 		}
// 		write (fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	close (fd);
// 	exec->fd_in = open(temp_file, O_RDONLY);
// 	unlink(temp_file);
// 	free(temp_file);
// 	if (fd < 0)
// 		return (error_message("heredoc", errno), 0);
// 	if (exec->fd_in != STDIN_FILENO)
// 		close(exec->fd_in);
// 	return (1);
// }