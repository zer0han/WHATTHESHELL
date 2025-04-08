/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/09 00:07:06 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_cd(t_token *args, t_envp *env)
{
	const char	*dir;

	if (!args->right)
	{
		dir = my_getenv("HOME", &env);
		if (!dir)
		{
			printf("cd: missing argument\n");
			return (EXIT_FAILURE);
		}
	}
	else
		dir = args->right->input;
	if (chdir(dir) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	cmd_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (EXIT_SUCCESS);
	}
	else
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
}

int	cmd_env(t_envp *env)
{
	while (env)
	{
		printf("%s\n", env->str);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
