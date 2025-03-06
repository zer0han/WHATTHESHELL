/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/06 17:28:56 by rdalal           ###   ########.fr       */
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
int	cmd_cd(t_token *args)
{
	const char	*dir;

	if (!args->right)
	{
		dir = getenv("HOME");
		if (!dir)
		{
			printf("cd: missing arguement\n");
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

/***pwd***/	
/*
	default behaviour of builtin pwd is same as using "pwd -L"
	pwd -L: prints the symbolic path
	pwd -P: prints the actual path
	use getcwd() function here
	getcwd(): string|false
*/
int	cmd_pwd(void)
{
	//(void)args;
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

/***env***/

/*
*if no flags or parameters specified, the env command displays your current environment, showing one Name=Value per line
**Print out a list of all the env variables

*'-i' or '-ignore-environment' or '-'
**runs a command with an empty environment

[-i | -] [Name=Value]... [Command [Argument ...]]
*/

static char	**dup_env(char **envp)
{
	char	**copy;
	int		count;
	int		i;

	count = 0;
	if(!envp)
		return (NULL);
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (++i < count)
		copy[i] = ft_strdup(envp[i]);
	copy[count] = NULL;
	return (copy);
}

int	cmd_env(char **envp)
{
	/*int		i;

	i = -1;
	while (envp[++i])
		ft_putendl_fd(envp[i], STDOUT_FILENO);
	return (EXIT_SUCCESS);*/
	char	**copy;
	int		i;
	int		j;
	char	*temp;

	copy = dup_env(envp);
	if (!copy)
		exit(EXIT_FAILURE);
	i = -1;
	while (copy[++i])
	{
		j = i;
		while (copy[++j])
		{
			if (ft_strcmp(copy[i], copy[j]) > 0)
			{
				temp = copy[i];
				copy[i] = copy[j];
				copy[j] = temp;
			}
		}
	}
	i = -1;
	while (copy[++i])
		printf("%s\n", copy[i]);
	free_array(copy);
	return (EXIT_SUCCESS);
}
