/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:44:07 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/19 17:29:52 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

/****functions needed*/
/* one for creating a pipe
 * one for handling child processes
 * one for executing cmds in child processes
 * one for checking if a cmd is an absolute path like (e.g /bin/ls)
 ** if it contains '/' it is an absolute path so we don't need path to lookup
* */

// static void	print_args(char **args)
// {
// 	int	i = 0;
// 	while (args[i])
// 		printf("%s\n", args[i++]);
// }

void	setup_child_process(t_exec *exec, char **envp)
{
	pid_t	pid;
	
	if (exec->next && pipe(exec->fd_pipe) == -1)
		return (perror("pipe failed"));
	pid = fork();
	if (pid == 0)
	{
		handle_pipe_redir(exec);
		//printf("DEBUGGED: Handled pipi\n");
		if (!get_path(exec->cmd))
			exit (127);
		//printf("DEBUG: why not handle caca?\n");
	//	printf("DEBUG path of cmd %s\n", get_path(exec->cmd));
		//print_args(exec->args);
		if (execve(get_path(exec->cmd), exec->args, envp) < 0)
			(printf("execve failed"));
		printf("DEBUG: passed execve\n");
		exit(1);
	}
	else if (pid > 0)
	{
		exec->pid = pid;
		if (exec->p_pipe >= 0)
			close(exec->p_pipe);
		if (exec->next)
		{
			close(exec->fd_pipe[1]);
			exec->next->p_pipe = exec->fd_pipe[0];
		}
	}
	else
		perror("fork");
}

void	wait_for_children(t_exec *exec)
{
	int	status;

	status = 0;
	while (exec)
	{
		waitpid(exec->pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		exec = exec->next;
	}
}

void	exec_pipeline(t_exec *exec, char **envp)
{
	t_exec	*head;

	head = exec;
	while (exec)
	{
		setup_child_process(exec, envp);
		// if (exec->p_pipe >= 0)
		// 	close(exec->p_pipe);
		exec = exec->next;
	}
	wait_for_children(head);
	while (head)
	{
		if (head->fd_pipe[0] != -1)
			close(head->fd_pipe[0]);
		if (head->fd_pipe[1] != -1)
			close (head->fd_pipe[1]);
		head = head->next;
	}
}
