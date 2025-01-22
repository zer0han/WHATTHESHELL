/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:41:24 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/20 16:27:19 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <strings.h>
#include <strings.h>

typedef struct s_token 
{
	char			*input;
	char			*type;
	int				*value;
	struct s_token	*left;
	struct s_token	*right;
	struct s_token	*next; 
}					t_token;

/*#include "minishell.h"*/

/*something like this this is just a very basic example*/

int	cmd_pwd(t_token **args)
{
	(void)args;
	/*
	default behaviour of builtin pwd is same as using "pwd -L"
	pwd -L: prints the symbolic path
	pwd -P: prints the actual path
	use getcwd() function here
	getcwd(): string|false
	*/
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	cmd_env(char **argv, char **envp)
{
	int		i;

	i = 0;
	(void)argv;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	cmd_echo(int argc, char **argv)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	if (argc > 1 && strcmp(argv[1], "-n") == 0)
	{
		nl = 0;
		i++;
	}
	while (i < argc)
	{
		printf("%s", argv[i]);
		if (i < argc - 1)
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}

int	cmd_cd(char **argv)
{
	const char	*dir;
	
	if (!argv[1])
	{
		dir = getenv("HOME");
		if (!dir)
		{	
			printf("cd: missing arguement\n");
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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*token_argv[1024];
	int		token_argc;

	while (1)
	{
		input = readline("whattheshell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		token_argc = 0;
		token_argv[token_argc] = strtok(input, " ");
		while (token_argv[token_argc] != NULL)
		{
			token_argc++;
			token_argv[token_argc] = strtok(NULL, " ");
		}
		if (strcmp(input, "pwd") == 0)
			cmd_pwd(NULL);
		else if (strcmp(input, "cd") == 0)
			cmd_cd(token_argv);
		else if (strcmp(input, "echo") == 0)
			cmd_echo(token_argc, token_argv);
		else if (strcmp(input, "env") == 0)
			cmd_env(token_argv, envp);
		else
			printf("Input entered: %s\n", input);
		free (input);
	}
	printf("Exiting whattheshell\n");
	return (0);
}

