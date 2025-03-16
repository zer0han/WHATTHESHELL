/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:50:45 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/16 16:03:46 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dispatch_cmds(t_token *tokens, t_envp *env, t_exec *exec_list)
{
	int	status;

	status = 0;
	if (!tokens || !tokens->input)
		return ;
	if (ft_strcmp(tokens->input, "echo") == 0)
		status = cmd_echo(tokens);
	else if (ft_strcmp(tokens->input, "cd") == 0)
		status = cmd_cd(tokens);
	else if (ft_strcmp(tokens->input, "pwd") == 0)
		status = cmd_pwd();
	else if (ft_strcmp(tokens->input, "env") == 0)
		status = cmd_env(env);
	else if (ft_strcmp(tokens->input, "export") == 0)
		status = cmd_export(env, &tokens);
	else if (ft_strcmp(tokens->input, "unset") == 0)
		status = cmd_unset(env, tokens);
	else if (ft_strcmp(tokens->input, "exit") == 0)
		status = cmd_exit(tokens, exec_list, env);
	g_exit_status = status;
	// return (status);
}

int	fd_is_builtin(t_token *token)
{
	char	*cmd;

	cmd = token->input;
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") \
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")\
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") \
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	execute_cmds(t_token *token, char **envp, t_envp *env, t_exec *exec_list)
{
	int		status;

	if (!token || !token->input)
		return ;
	status = 0;
	if (exec_list->redir)
		redirection_process(exec_list, exec_list->redir, envp);
	if (fd_is_builtin(token))
		dispatch_cmds(token, env, exec_list);
	else
		exec_external(token, envp);
	if (g_exit_status != 0)
		return ;
}
