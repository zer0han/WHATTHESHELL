/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 19:35:41 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	*create_exec(t_token *cmd_token)
{
	t_exec		*exec_cmd;

	if (!cmd_token)
		return (NULL);
	exec_cmd = malloc(sizeof(t_exec));
	if (!exec_cmd)
		return (NULL);
	exec_cmd->cmd_token = cmd_token;
	exec_cmd->cmd = cmd_token->input;
	exec_cmd->redir = init_redir(&cmd_token);
	exec_cmd->args = NULL;
	exec_cmd->fd_in = STDIN_FILENO;
	exec_cmd->fd_out = STDOUT_FILENO;
	exec_cmd->fd_pipe[0] = -1;
	exec_cmd->fd_pipe[1] = -1;
	exec_cmd->p_pipe = -1;
	exec_cmd->pid = -1;
	exec_cmd->next = NULL;
	exec_cmd->prev = NULL;
	exec_cmd->is_pipeline = 0;
	return (exec_cmd);
}

static void	process_args(t_exec *exec_cmd, t_token *node)
{
	int	arg_size;
	int	i;

	if (!exec_cmd || !exec_cmd->cmd || !exec_cmd->cmd[0])
	return (ft_putstr_fd("ERROR: exec->cmd is NULL!\n", STDERR_FILENO));
	arg_size = count_args(node);
	exec_cmd->args = malloc(sizeof(char *) * (arg_size + 2));
	if (!exec_cmd->args)
		return (perror("malloc failed in process_args"));
	exec_cmd->args[0] = ft_strdup(exec_cmd->cmd);
	if (!exec_cmd->args[0])
		return (perror("strdup failed for cmd"));
	i = 1;
	while (node && (ft_strcmp(node->type, "arg") == 0 \
		|| ft_strcmp(node->type, "option") == 0))
	{
		exec_cmd->args[i] = ft_strdup(node->input);
		if (!exec_cmd->args[i])
		{
			while (--i >= 0)
				free (exec_cmd->args[i]);
			exec_cmd->args = NULL;
			return (free_exec(exec_cmd));
		}
		node = node->right;
		i++;
	}
	exec_cmd->args[i] = NULL;
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
			if (!new_exec)
				return (NULL);
			process_args(new_exec, current_token->right);
			if (exec_list)
				exec_list->is_pipeline = 1;
			add_exec_node(&exec_list, new_exec);
		}
		current_token = current_token->right;
	}
	return (exec_list);
}

t_exec	*main_execution(t_token **token_tree, t_envp *env)
{
	t_exec	*exec_list;
	int		stdin_save;
	int		stdout_save;

	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	exec_list = create_exec_list(*token_tree);
	if (!exec_list)
		return (NULL);
	if (exec_list->is_pipeline)
		exec_pipeline(exec_list, env);
	else
	{
		if (apply_redirection(exec_list))
		{
			setup_redir(exec_list);
			if (fd_is_builtin(exec_list->cmd_token))
				execute_cmds(exec_list->cmd_token, NULL, env, exec_list);
			else
				exec_external(exec_list->cmd_token, NULL, env, exec_list);
		}
	}
	dup2(stdin_save, STDIN_FILENO);
	dup2(stdout_save, STDOUT_FILENO);
	close(stdin_save);
	close(stdout_save);
	return (exec_list);
}

// int	is_pipe()

// t_exec	*main_execution(t_token **token_tree, char **envp, t_envp *env)
// {
// 	t_exec	*exec_list;
// 	t_token	*temp;
// 	pid_t	pid;
	
// 	pid = 0;
// 	if (!token_tree || !*token_tree)
// 		return (NULL);
// 	temp = *token_tree;
// 	exec_list = create_exec_list(temp);
// 	if (!exec_list)
// 		return (NULL);
// 	//if (exec_list->next)
// 	exec_pipeline(exec_list, envp, env);
// 	// if (fd_is_builtin(exec_list->cmd_token))
// 	// 	execute_cmds(exec_list->cmd_token, envp, env, exec_list);
// 	// else
// 	// 	exec_external(exec_list->cmd_token, envp, env, exec_list);
// 	return (exec_list);
// }
