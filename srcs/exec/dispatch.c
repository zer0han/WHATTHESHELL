/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:50:45 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/25 19:48:06 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_cmds(t_token *tokens, t_data *code, char ***envp)
{
	int		status;

	status = 0;
	if (!tokens || !tokens->input)
		return ;
	if (ft_strcmp(tokens->input, "echo") == 0)
		status = cmd_echo(tokens);
	else if (ft_strcmp(tokens->input, "cd") == 0)
		status = cmd_cd(tokens);
	else if (ft_strcmp(tokens->input, "pwd") == 0)
		status = cmd_pwd(tokens);
	else if (ft_strcmp(tokens->input, "env") == 0)
		status = cmd_env(tokens, *envp);
	else if (ft_strcmp(tokens->input, "export") == 0)
		status = cmd_export(envp, tokens);
	else if (ft_strcmp(tokens->input, "unset") == 0)
		status = cmd_unset(envp, tokens);
	else if (ft_strcmp(tokens->input, "exit") == 0)
		status = cmd_exit(code, tokens);
	return (status);
}

static int	fd_is_builtin(t_token *token)
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

void	execute_cmds(t_token *token, t_data *data, char **envp)
{
	int		status;
	char	*cmd;

	if (!token || !token->input)
		return ;
	status = 0;
	cmd = token->input;
	if (fd_is_builtin(token))
		status = dispatch_cmds(token, data, envp);
	else
		exec_external(token, *envp);
	data->nbr = status;
	if (status != 0)
		free_errors(&token);
}
