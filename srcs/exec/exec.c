/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/18 19:41:44 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*just pseudo code examples of how the exec funcitons will look*/

int	eval(t_token node)
{
	if (node.type == PLUS)
		return (node.left + node.right);
	else
		return (node.value);
}

int	main_execution(t_token *token_tree, t_data *code, char ***envp)
{
	if (!token_tree)
		return (0); // Nothing to execute
	if (token_tree->type == PIPE) // Handle pipeline
	{
		t_exec *exec_list = convert_token_to_exec_list(token_tree);
		exec_pipeline(exec_list, envp);
		free_exec_list(exec_list);
	}
	else // Handle simple command
	{
		t_exec *simple_exec = create_simple_exec_from_token(token_tree);
		redirection_process(simple_exec->redir);
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_simple_exec(simple_exec);
			return (1);
		}
		if (pid == 0) // Child process
		{
			dispatch_cmds(simple_exec->redir, code, envp); // Dispatch handles built-ins/external
			exit(0);
		}
		else // Parent process
		waitpid(pid, NULL, 0);
		free_simple_exec(simple_exec);
	}
	return (0);
}

/*
function convert_token_to_exec_list(token_tree):
  exec_list = empty list
  current_token = token_tree

  while current_token is not NULL:
    if current_token.type is PIPE:
      # Process command before the pipe (left side)
      command_exec = create_exec_for_command(current_token.left) # Assume this function exists
      add command_exec to exec_list
      current_token = current_token.right # Move to command after pipe (right side)
    else: # It's a simple command (or last part of pipe)
      command_exec = create_exec_for_command(current_token) # Assume this function exists
      add command_exec to exec_list
      current_token = NULL # End of command/pipeline

  return exec_list

function create_exec_for_command(cmd_token):
  exec_cmd = create new t_exec struct
  exec_cmd.cmd = cmd_token.input      # Command name
  # ... (set up args, redirections, file descriptors in t_exec based on cmd_token) ...
  return exec_cmd
*/

int	execute_pipe(void)
{
	pipe(pipe_fds);
	left_pid = fork();
	if (pid == 0) //child0
	{
		// do stuff
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	else //back to the parent
	{
		// do the same thing for child1
		//make sure to close the pipe_fds in the parent too
	}
}

/*basically open a file and redirect using dup2 the STDIN or STDOUT to that file
*and in case of heredoc we redirect the STDIN to a pipe (|)
*/