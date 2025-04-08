/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_more.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:30:38 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:37:08 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_heredoc(t_exec *exec)
{
	int		fd;
	char	*pid;

	pid = ft_itoa(getpid());
	if (pid)
		exec->heredoc_file = ft_strjoin("/tmp/.heredoc", pid);
	if (!pid || !exec->heredoc_file)
		return (free(pid), error_message("heredoc", ENOMEM), -1);
	free(pid);
	fd = open(exec->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(exec->heredoc_file);
		return (error_message("heredoc", errno), g_exit_status = 1, -1);
	}
	return (fd);
}

int	handle_heredoc(t_redir *redir, t_exec *exec)
{
	int		fd;
	char	*l;
	int		i;

	i = 0;
	fd = init_heredoc(exec);
	if (fd < 0)
		return (0);
	while (redir->delimiter[i])
	{
		l = readline("> ");
		if (l && !redir->delimiter[i + 1] && ft_strcmp(l, redir->delimiter[i]))
		{
			write (fd, l, ft_strlen(l));
			write(fd, "\n", 1);
			free(l);
		}
		else if (!l || !ft_strcmp(l, redir->delimiter[i]))
			i++;
	}
	close(fd);
	exec->fd_in = open(exec->heredoc_file, O_RDONLY);
	if (exec->fd_in < 0)
		return (free(l), error_message("herdoc", errno), g_exit_status = 1, 0);
	return (free(l), 1);
}

static t_redir	*create_redir_node(t_redir **tail)
{
	t_redir	*new;

	new = ft_calloc(1, sizeof(t_redir));
	*tail = ft_last_redir_node(tail);
	if (!new)
		return (NULL);
	if (!*tail)
	{
		new->prev = NULL;
		*tail = new;
	}
	else
	{
		new->prev = *tail;
		new->prev->next = new;
	}
	return (new);
}

static void	assign_redir_node(t_token **node, t_redir *new)
{
	if (ft_strcmp((*node)->input, ">") == 0)
		new->type = REDIR_OUT;
	else if (ft_strcmp((*node)->input, ">>") == 0)
		new->type = REDIR_APPEND;
	else if (ft_strcmp((*node)->input, "<") == 0)
		new->type = REDIR_IN;
	else if (ft_strcmp((*node)->input, "<<") == 0)
		new->type = HEREDOC;
}

int	parse_redir_node(t_token **node, t_redir **tail)
{
	t_redir	*new;

	new = create_redir_node(tail);
	if (!new)
		return (0);
	if (!(*node)->right || !(*node)->right->input)
	{
		free(new);
		ft_putstr_fd("Syntax error\n", 2);
		return (0);
	}
	assign_redir_node(node, new);
	if (new->type == HEREDOC && !create_delim_array(new, node))
		return (0);
	else
		new->file = ft_strdup((*node)->right->input);
	if (new->type != HEREDOC)
		*node = (*node)->right->right;
	new->next = NULL;
	return (1);
}
