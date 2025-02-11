/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrnl_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:31:21 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/11 16:37:56 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*call your the path functions here */
char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path)
{
	char	**argv;
	int		argc;
	int		i;
	t_token	*arg;

	arg = tokens;
	argc = 0;
	while (arg)
	{
		argc++;
		arg = arg->right;
	}
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	arg = tokens;
	i = 0;
	while (i < argc)
	{
		argv[i] = expand_variables(arg->input);
		arg = arg->right;
	}
	argv[argc] = NULL;
	*cmd_path = get_path(argv[0]);
	if (!(*cmd_path))
	{
		printf("command not found:%s\n", argv[0]);
		free_array(argv);
		return (NULL);
	}
	return (argv);
}

void	run_cmd(char *cmd_path, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid = 0)
	{
		if (execve(cmd_path, argv, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else if (pid > 0)
		wait (NULL);
	else
		perror("fork");
	free(cmd_path);
	free_array(argv);
}

void	exec_external(t_token *tokens, char **envp)
{
	char	*cmd_path;
	char	**argv;

	argv = cmd_prep(tokens, envp, &cmd_path);
	if (!argv)
		return ;
	run_cmd(cmd_path, argv, envp);
}