/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/28 18:10:33 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*just error and basic struct here for the builtins can expand more on them
also have to write a function for the executions of the builtins*/

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


/***unset***/
 /* unset [variable_name]
 * unset [-options] [variable_name]
 ** undefine a variable in bash
 * unset -f [function_name]
 ** undefine shell function in bash
 */

int	cmd_env(char **argv, char **envp)
{
	int		i;
	char	*env_variable;
	char	*key;
	char	*value;

	i = 0;
	env_variable = envp[i];
	while (env_variable)
	{
		ft_printf("%s\n", env_variable);
		key = strtok(env_variable, "=");
		value = strtok(NULL, "=");
		if (strcmp(key, "PATH") == 0)
			ft_prinf("%s\n", value);
		i++;
		env_variable = envp[i];
	}
	return (0);
}
