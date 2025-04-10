/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/10 22:38:06 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*update_pwds(t_envp **env)
{
	char	*eq;
	char	cwd[1024];
	t_envp	*pwd;

	pwd = *env;
	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	while (pwd && ft_strncmp("PWD", pwd->str, 3))
		pwd = pwd->next;
	eq = ft_strchr(pwd->str, '=') + 1;
	if (update_env(env, "OLDPWD", eq))
		return (NULL);
	if (update_env(env, "PWD", cwd))
		return (NULL);
	return (env);
}

int	cmd_cd(t_token *args, t_envp **env)
{
	const char	*dir;

	if (!args->right)
	{
		dir = my_getenv("HOME", env);
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
	update_pwds(env);
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
