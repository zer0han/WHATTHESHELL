/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:15:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/16 18:18:18 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redir(t_exec *exec)
{
	if (exec->redir)
	{
		printf("DEBUG calling redirection process\n");
		redirection_process(exec, exec->redir);
	}
	printf("DEBUGG SETTING UP REDIRECTION\n");
	if (exec->fd_in != STDIN_FILENO)
	{
		printf("DEBUGG REDIRECTION STDIN TO FD %d\n", exec->fd_in);
		if (dup2(exec->fd_in, STDIN_FILENO) == -1)
		{
			perror ("ERROR dup2 failed for stdin");
			exit(1);
		}
		close(exec->fd_in);
	}
	if (exec->fd_out != STDOUT_FILENO)
	{
		printf("DEBUGGGG REDIR STDOUT TO FD %d\n", exec->fd_out);
		if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
		{
			perror("ERROR dup2 failed for stdout");
			exit(1);
		}
		close(exec->fd_out);
	}
	printf("DEBUG FINISHED SETTING UP REDIRECTION\n");
}



void	clean_fds(t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
	{
		printf("DEBUG: Closing input FD %d\n", exec->fd_in);
		close(exec->fd_in);
	}
	if (exec->fd_out != STDOUT_FILENO)
	{
		printf("DEBUG: Closing output FD %d\n", exec->fd_out);
		close(exec->fd_out);
	}
	if (exec->p_pipe >= 0)
	{
		printf("DEBUG: Closing pipe FD %d\n", exec->p_pipe);
		close(exec->p_pipe);
	}
	if (exec->next)
	{
		printf("DEBUG: Closing pipe write FD %d\n", exec->fd_pipe[1]);
		close(exec->fd_pipe[1]);
	}
}

void	execute_child_redir(t_exec *exec, char **envp)
{
	char	*path;

	setup_redir(exec);
	printf("DEBUGG FINISHED SETTING UP REDIRECTION\n");
	if (exec->cmd)
	{
		path = get_path(exec->cmd);
		if (!path)
		{
			ft_putstr_fd("WHATTHESHELL: ", STDERR_FILENO);
			ft_putstr_fd(exec->cmd, STDERR_FILENO);
			exit(127);
		}
		printf("DEBUG RUNNING EXECVE %s\n", path);
		execve(path, exec->args, envp);
		perror("ERROR execve failed");;
		exit (1);
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
