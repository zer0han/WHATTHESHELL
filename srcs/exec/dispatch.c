/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:50:45 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/04 17:53:13 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_cmds(t_token *tokens, char ***envp, t_exec *exec_list)
{
	int		status;

	status = 0;
	//printf("len of the input: %zu\n", ft_strlen(tokens->input));
	if (!tokens || !tokens->input)
		return (1);
	if (ft_strncmp(tokens->input, "echo", ft_strlen(tokens->input)) == 0)
	{
		status = cmd_echo(tokens);
		//printf("the best debugger\n");
	}
	else if (ft_strncmp(tokens->input, "cd", ft_strlen(tokens->input)) == 0)
	{	status = cmd_cd(tokens);
		printf("thhe best debugger\n");
	}
	else if (ft_strncmp(tokens->input, "pwd", ft_strlen(tokens->input)) == 0)
	{
		status = cmd_pwd();
		printf("work bitch\n");
	}
		
	else if (ft_strcmp(tokens->input, "env") == 0)
		status = cmd_env(*envp);
	else if (ft_strcmp(tokens->input, "export") == 0)
	{
		status = cmd_export(envp, tokens);
		printf("this should work\n");
	}
	else if (ft_strcmp(tokens->input, "unset") == 0)
		status = cmd_unset(envp, tokens);
	else if (ft_strcmp(tokens->input, "exit") == 0)
		status = cmd_exit(tokens, exec_list);
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

void	execute_cmds(t_token *token, char **envp, t_exec *exec_list)
{
	int		status;

	if (!token || !token->input)
		return ;
	status = 0;
	if (fd_is_builtin(token))
		dispatch_cmds(token, &envp, exec_list);
	else
	//	exec_external(token, envp);
	if (status != 0)
		free_tokens(token);
}
