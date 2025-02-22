/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/22 16:49:07 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*just pseudo code examples of how the exec funcitons will look*/

t_exec	*create_exec(t_token *cmd_token)
{
	t_exec	*exec_cmd;
	t_token	*node;

	exec_cmd = malloc(sizeof(t_exec));
	if (!exec_cmd)
		return (NULL);
	exec_cmd->cmd = cmd_token->input;
	exec_cmd->args = NULL;
	exec_cmd->redir = cmd_token->redir;
	exec_cmd->fd_in = STDIN_FILENO;
	exec_cmd->fd_out = STDOUT_FILENO;
	exec_cmd->next = NULL;
	node = cmd_token;
	while (node && ft_strcmp(node->type, "cmd") != 0)
		node = node->right;
	if (!node)
		return (exec_cmd);
	node = node->right;
	while (node && (ft_strcmp(node->type, "arg") == 0 \
		|| ft_strcmp(node->type, "option") == 0))
	{
		exec_cmd->args = node->input;
		node = node->right;
	}
	return (exec_cmd);
}

t_exec	*create_exec_list(t_token *token_tree)
{
	t_exec	*exec_list;
	t_exec	*new_exec;
	t_exec	*last_exec;
	t_token	*current_token;

	exec_list = NULL;
	last_exec = NULL;
	current_token = token_tree;
	while (current_token)
	{
		if(ft_strcmp(current_token->type, "redir_in") == 0 || \
			ft_strcmp(current_token->type, "redir_out") == 0) 
		{
			handle_redirection(current_token);
			current_token = current_token->right;
		}
		if (ft_strcmp(current_token->type, "cmd") == 0)
			new_exec = crete_exec(current_token);
		else
			break ; //stop if no valid command
		new_exec->prev = last_exec;
		if (last_exec)
			last_exec->next = new_exec;
		if (!exec_list)
			exec_list = new_exec;
		last_exec = new_exec;
		current_token = current_token->right; //move to the next token
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
	handle_redirection(temp);
	exec_list = create_exec_list(temp);
	if (exec_list->next)
		exec_pipeline(exec_list, envp);
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), free_exec(exec_list));
		if (pid == 0)
		{
			dispatch_cmds(exec_list->redir, code, envp);
			exit (0);
		}
		else
			waitpid(pid, NULL, 0);
		free_exec(exec_list);
	}
}

//still not complete
void	handle_redirection(t_token *token)
{
	int		fd;
	t_token	*last_redir;

	last_redir = NULL;
	while (token)
	{
		if (ft_strcmp(token->type, "redir_out") == 0 || \
			ft_strcmp(token->type, "redir_append") == 0)
			last_redir = token;
		token = token->right;
	}
	if (last_redir)
	{
		if (ft_strcmp(last_redir->type, "redir_out") == 0)
			fd = open(last_redir->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(last_redir->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd = -1)
			perror ("failed open");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close (fd);
		}
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
