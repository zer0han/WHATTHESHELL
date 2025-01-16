/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:41:24 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/16 13:11:33 by rdalal           ###   ########.fr       */
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

int	cmd_echo(char **argv)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	if (argv[1] && strcmp(argv[1], "-n") == 0)
	{
		nl = 0;
		i = 2;
	}
	while (argv[1])
	{
		if ((i > 2) || i == 2 && nl)
			printf(" ");
		printf("%s", argv[i]);
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

int	main(void)
{
	char	*input;
	char	*argv[3];

	while (1)
	{
		input = readline("whattheshell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		argv[0] = strtok(input, " ");
		argv[1] = strtok(NULL, " ");
		argv[2] = NULL;
		if (strcmp(input, "pwd") == 0)
			cmd_pwd(NULL);
		else if (strcmp(input, "cd") == 0)
			cmd_cd(argv);
		else if (strcmp(input, "echo") == 0)
			cmd_echo(argv);
		else
			printf("Input entered: %s\n", input);
		free (input);
	}
	printf("Exiting whattheshell\n");
	return (0);
}
