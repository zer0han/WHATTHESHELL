/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/19 16:02:46 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*just pseudo code examples of how the exec funcitons will look*/

t_exec	*create_exec(t_token *cmd_token)
{
	t_exec	*exec_cmd;

	exec_cmd = malloc(sizeof(t_exec));
	if (!exec_cmd)
		return (NULL);
	exec_cmd->cmd = cmd_token->input;
	exec_cmd->args = //parse_args(cmd_token) converts token args into list
	exec_cmd->redir = cmd_token->redir;
	exec_cmd->fd_in = STDIN_FILENO;
	exec_cmd->fd_out = STDOUT_FILENO;
	exec_cmd->next = NULL;
	handle_redirection(exec_cmd);
	return (exec_cmd);
}

t_exec	*create_exec_list(t_token *token_tree)
{
	t_exec	*exec_list;
	t_exec	*new_exec;
	t_exec	*prev_exec;
	t_token	*current_token;

	exec_list = NULL;
	prev_exec = NULL;
	current_token = token_tree;
	while (current_token)
	{
		if (ft_strcmp(current_token->type, "pipe") == 0)
			new_exec = create_exec(current_token->left);
		else
			new_exec = create_exec(current_token);
		if (!exec_list)
			exec_list = new_exec;
		else
			prev_exec->next = new_exec;
		prev_exec = new_exec;
		if (ft_strccmp(current_token->type, "pipe") == 0)
			current_token = current_token->right;
		else
			current_token = NULL;
	}
	return (exec_list);
}

void	main_execution(t_token *token_tree, t_data *code, char ***envp)
{
	t_exec	*exec_list;
	t_exec	*exec;
	pid_t	pid;
	
	if (!token_tree)
		return ;
	if (ft_strcmp(token_tree->type, "pipe") == 0)
	{
		exec_list = create_exec_list(token_tree);
		exec_pipeline(exec_list, envp);
		free_exec(exec_list);
	}
	else
	{
		exec = create_exec(token_tree);
		handle_redirection(exec);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), free_exec(exec));
		if (pid == 0)
		{
			dispatch_cmds(exec->redir, code, envp);
			exit (0);
		}
		else
			waitpid(pid, NULL, 0);
		free_exec(exec);
	}
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
