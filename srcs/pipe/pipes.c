/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:44:07 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 17:16:58 by rdalal           ###   ########.fr       */
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

// void	setup_child_process(t_exec *exec, char **envp)
// {
// 	pid_t	pid;
	
// 	if (exec->next && pipe(exec->fd_pipe) == -1)
// 		return (perror("pipe failed"));
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		handle_pipe_redir(exec);
// 		if (!get_path(exec->cmd))
// 			exit (127);
// 		if (execve(get_path(exec->cmd), exec->args, envp) < 0)
// 			(printf("execve failed"));
// 		printf("DEBUG: passed execve\n");
// 		exit(1);
// 	}
// 	else if (pid > 0)
// 	{
// 		exec->pid = pid;
// 		if (exec->p_pipe >= 0)
// 			close(exec->p_pipe);
// 		if (exec->next)
// 		{
// 			close(exec->fd_pipe[1]);
// 			exec->next->p_pipe = exec->fd_pipe[0];
// 		}
// 	}
// 	else
// 		perror("fork");
// }

// void	setup_child_process(t_exec *exec, t_envp *env)
// {
// 	pid_t pid;

// 	if (exec->next && pipe(exec->fd_pipe) == -1)
// 	{
// 		perror("pipe failed");
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork failed");
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		if (exec->next)
// 			close(exec->fd_pipe[0]);
// 		child_process(exec, env);
// 		exit(0);
// 	}
// 	else
// 	{
// 		exec->pid = pid;
// 		if (exec->p_pipe >= 0)
// 			close(exec->p_pipe);
// 		if (exec->next)
// 		{
// 			close(exec->fd_pipe[1]);
// 			exec->next->p_pipe = exec->fd_pipe[0];
// 		}
// 		else
// 			waitpid(pid, NULL, 0);
// 	}
// }

// void	wait_for_children(t_exec *exec)
// {
// 	int	status;

// 	status = 0;
// 	while (exec)
// 	{
// 		//waitpid(exec->pid, &status, 0);
// 		if (WIFEXITED(status))
// 			g_exit_status = WEXITSTATUS(status);
// 		exec = exec->next;
// 	}
// }

// void	exec_pipeline(t_exec *exec, char **envp, t_envp *env)
// {
// 	t_exec	*head;

// 	head = exec;
// 	while (exec)
// 	{
//         fprintf(stderr, "DEBUG [exec_pipeline] before setup of pipes\n");
// 		setup_child_process(exec, envp, env);
//         fprintf(stderr, "DEBUG [exec_pipeline] pipes have been set up and ready\n");
// 		if (exec->p_pipe >= 0)
// 			close(exec->p_pipe);
//         if (exec->next)
//             close (exec->fd_pipe[1]);
// 		exec = exec->next;
// 	}
// 	wait_for_children(head);
// 	while (head)
// 	{
// 		if (head->p_pipe != -1)
// 			close(head->p_pipe);
// 		if (head->fd_pipe[0] != -1)
// 			close(head->fd_pipe[0]);
// 		if (head->fd_pipe[1] != -1 && head->next == NULL)
// 			close (head->fd_pipe[1]);
// 		head = head->next;
// 	}
// }
	// t_exec *head = exec;

    // // Setup all child processes
    // while (exec)
    // {
    //     setup_child_process(exec, envp, env);
    //     exec = exec->next;
    // }

    // // Wait for all children to finish
    // wait_for_children(head);

    // // Close all pipe FDs
    // while (head)
    // {
    //     if (head->fd_pipe[0] != -1)
    //         close(head->fd_pipe[0]);
    //     if (head->fd_pipe[1] != -1)
    //         close(head->fd_pipe[1]);
    //     head = head->next;
    // }
// }

// void	setup_child_process(t_exec *exec, t_envp *env)
// {
// 	pid_t	pid;

// 	if (exec->next && pipe(exec->fd_pipe) == -1)
// 	{
// 		perror("WHATTHESHELL: PIPE");
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("WHATTHESHELL: FORK");
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		child_process(exec, env);
// 		exit(g_exit_status);
// 	}
// 	else
// 	{
// 		exec->pid = pid;
// 		if (exec->p_pipe != -1)
// 			close(exec->p_pipe);
// 		if (exec->next)
// 		{
// 			close(exec->fd_pipe[1]);
// 			exec->next->p_pipe = exec->fd_pipe[0];
// 		}
// 	}
// }

void	setup_child_process(t_exec *exec, t_envp *env)
{
	pid_t	pid;

	if (exec->next && pipe(exec->fd_pipe) == -1)
		return (perror("pipe failed"));
	pid = fork();
	if (pid == 0)
	{
		child_process(exec, env);
		exit(0);
	}
	else
	{
		exec->pid = pid;
		if (exec->p_pipe != -1)
			close(exec->p_pipe);
		if (exec->next)
		{
			close(exec->fd_pipe[1]);
			exec->next->p_pipe = exec->fd_pipe[0];
		}
	}
}

void	wait_for_children(t_exec *exec)
{
	int	status;

	while (exec)
	{
		if (exec->pid != -1)
		{
			waitpid(exec->pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
		exec = exec->next;
	}
}

// Modified exec_pipeline
void    exec_pipeline(t_exec *exec, t_envp *env)
{
    t_exec  *head = exec;
    int     pipe_fd[2];

    while (exec && exec->next)
    {
        if (pipe(pipe_fd) < 0)
            handle_error("pipe", errno, NULL);
        exec->fd_pipe[0] = pipe_fd[0];
        exec->fd_pipe[1] = pipe_fd[1];
        setup_child_process(exec, env);
        close(exec->fd_pipe[1]);
        exec = exec->next;
    }
    if (exec)
        setup_child_process(exec, env);
    wait_for_children(head);
}

// void	exec_pipeline(t_exec *exec, t_envp *env)
// {
// 	t_exec	*head;

// 	head = exec;
// 	while (exec)
// 	{
// 		if (exec->is_pipeline)
// 		{
// 			setup_child_process(exec, env);
// 			if (exec->p_pipe != -1)
// 				close(exec->p_pipe);
// 			if (exec->next)
// 				close(exec->fd_pipe[1]);
// 		}
// 		exec = exec->next;
// 	}
// 	wait_for_children(head);
// }