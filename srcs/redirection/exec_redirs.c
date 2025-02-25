/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:15:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/25 19:09:08 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redir(t_exec *exec)
{
	if (exec->redir)
		redirection_process(exec->redir);
	if (exec->fd_in != STDIN_FILENO)
		dup2(exec->fd_in, STDIN_FILENO);
	if (exec->fd_out != STDOUT_FILENO)
		dup2(exec->fd_out, STDOUT_FILENO);
}

void	clean_fds(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDOUT_FILENO)
		close(exec->fd_out);
}

void	execute_child_redir(t_exec *exec, char **envp)
{
	char	*path;

	setup_redirections(exec);
	if (exec->cmd)
	{
		path = get_path(exec->cmd);
		if (!path)
		{
			ft_putstr("whattheshell: ", STDERR_FILENO);
			ft_putstr(exec->cmd, STDERR_FILENO);
			ft_putendl_fd("; cmd not found", STDERR_FILENO);
			exit(127);
		}
		execve(path, exec->args, envp);
		free(path);
	}
	exit(EXIT_SUCCESS);
}

static int	fork_safe(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("whattheshell: fork");
		return (-1);
	}
	return (0);
}

void	redir_execute_cmd(t_exec *exec, char **envp)
{
	int		status;

	if (fork_safe(&exec->pid) == -1)
		return ;
	if (exec->pid == 0)
		execute_child_redir(exec, envp);
	else
	{
		waitpid(exec->pid, &status, 0);
		clean_fds(exec);
	}
}
