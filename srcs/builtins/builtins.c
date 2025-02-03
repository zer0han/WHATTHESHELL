/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/29 19:34:24 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
cd [options] [directory]

cd
cd -- in the POSIX standard, cd -- behaves like runing cd without arguments
cd - ()
cd ..
cd /
cd ~

int chdir(const char *path);
*/
int	cmd_cd(char **argv)
{
	const char	*dir;

	if (!argv[1])
	{
		dir = getenv("HOME");
		if (!dir)
		{
			ft_printf("cd: missing arguement\n");
			return (1);
		}
	}
	else
		dir = argv[1];
	if (chdir(dir) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

/***pwd***/	
/*
	default behaviour of builtin pwd is same as using "pwd -L"
	pwd -L: prints the symbolic path
	pwd -P: prints the actual path
	use getcwd() function here
	getcwd(): string|false
*/
int	cmd_pwd(char **args)
{

	(void)args;
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

/***env***/

/*
*if no flags or parameters specified, the env command displays your current environment, showing one Name=Value per line
**Print out a list of all the env variables

*'-i' or '-ignore-environment' or '-'
**runs a command with an empty environment

[-i | -] [Name=Value]... [Command [Argument ...]]
*/
int	cmd_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		sort_export_env(envp);
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
