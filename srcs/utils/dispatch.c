/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:50:45 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/12 17:47:49 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dispatch_cmds(t_token *tokens, t_data *code, char ***envp)
{
	if (!tokens || !tokens->input)
		return ;
	if (ft_strcmp(tokens->input, "echo") == 0)
		cmd_echo(tokens);
	else if (ft_strcmp(tokens->input, "cd") == 0)
		cmd_cd(tokens);
	else if (ft_strcmp(tokens->input, "pwd") == 0)
		cmd_pwd(tokens);
	else if (ft_strcmp(tokens->input, "env") == 0)
		cmd_env(tokens, *envp);
	else if (ft_strcmp(tokens->input, "export") == 0)
		cmd_export(envp, tokens);
	else if (ft_strcmp(tokens->input, "unset") == 0)
		cmd_unset(envp, tokens);
	else if (ft_strcmp(tokens->input, "exit") == 0)
		cmd_exit(code, tokens);
	else
		exec_external(tokens, *envp);
}

