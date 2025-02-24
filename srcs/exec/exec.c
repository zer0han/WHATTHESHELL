/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/24 20:47:35 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*just pseudo code examples of how the exec funcitons will look*/

/*signal hanldig
signal(SIGINT, SIG_IGN)
signal(SIGQUIT, SIG_IGN)
*/

t_exec	*create_exec(t_token *cmd_token)
{
	t_exec	*exec_cmd;

	exec_cmd = malloc(sizeof(t_exec));
	if (!exec_cmd)
		return (NULL);
	exec_cmd->cmd_token = cmd_token;
	exec_cmd->cmd = cmd_token->input;
	exec_cmd->redir = cmd_token->redir;
	exec_cmd->fd_in = STDIN_FILENO;
	exec_cmd->fd_out = STDOUT_FILENO;
	return (exec_cmd);
}

static void	process_args(t_exec *exec_cmd, t_token *node)
{
	while (node && (ft_strcmp(node->type, "arg") == 0 \
		|| ft_strcmp(node->type, "option") == 0))
	{
		add_args(exec_cmd, node->input);
		node = node->right;
	}
	
}

t_exec	*create_exec_list(t_token *token_tree)
{
	t_exec	*exec_list;
	t_exec	*new_exec;
	t_token	*current_token;

	exec_list = NULL;
	current_token = token_tree;
	while (current_token)
	{
		if (ft_strcmp(current_token->type, "cmd") == 0)
		{
			new_exec = create_exec(current_token);
			process_args(new_exec, current_token->right);
			add_exec_node(&exec_list, new_exec);
		}
		current_token->right;
	}
	return (exec_list);

}

void	main_execution(t_token **token_tree, t_data *code, char ***envp)
{
	t_exec	*exec_list;
	t_token	*temp;
	pid_t	pid;
	
	if (!token_tree || !*token_tree)
		return ;
	temp = *token_tree;
	redirection_process(exec_list->redir);
	exec_list = create_exec_list(temp);
	if (!exec_list)
		return ;
	if (exec_list->next)
		exec_pipeline(exec_list, envp);
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), free_exec(exec_list));
		if (pid == 0)
			dispatch_cmds(exec_list->cmd_token, code, envp);
		else
			waitpid(pid, &code->nbr, 0);

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
