/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrnl_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:31:21 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/19 19:30:46 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*call your the path functions here */
/*fix this bullshit doesn't work causes leaks*/

static int	ft_count_args(t_token *tokens)
{
	int		count;
	t_token	*temp;

	count = 0;
	temp = tokens;
	while (temp  && ((ft_strcmp(temp->type, "arg") == 0) || \
	ft_strcmp(temp->type, "option") == 0 || ft_strcmp(temp->type, "cmd") == 0))
	{
		count++;
		temp = temp->right;
	}
	return (count);
}

static char	**fill_argv(t_token *tokens, int argc, t_envp *env)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < argc && tokens)
	{
		argv[i] = expand_variables(tokens->input, env);
		if (!argv[i])
		{
			free_array(argv);
			return (NULL);
		}
		tokens = tokens->right;
		i++;
	}
	argv[argc] = NULL;
	return (argv);
}

static char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path, t_envp *env)
{
	char	**argv;
	int		argc;

	(void)envp;
	argc = ft_count_args(tokens);
	argv = fill_argv(tokens, argc, env);
	if (!argv)
		return (NULL);
	*cmd_path = get_path(argv[0]);
	if (!*cmd_path)
	{
		ft_putstr_fd("WHATTHESHELL: cmd not found\n", STDERR_FILENO);
		free_array(argv);
		return (NULL);
	}
	return (argv);
}

void	run_cmd(char *cmd_path, char **argv, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (free_array(argv));
	}
	else if (pid == 0)
	{
		execve(cmd_path, argv, envp);
		ft_putstr_fd("WHATTHESHELL: \n", STDERR_FILENO);	
		free(cmd_path);
		free_array(argv);
		exit(127);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

// void	exec_external(t_token *tokens, char **envp, t_envp *env)
// {
// 	char	*cmd_path;
// 	char	**argv;

// 	cmd_path = NULL;
// 	argv = cmd_prep(tokens, envp, &cmd_path, env);
// 	if (!argv)
// 	{
// 		g_exit_status = 127;
// 		return ;
// 	}
// 	run_cmd(cmd_path, argv, envp);
// 	printf("it reaches here\n");
// 	free(cmd_path);
// 	free_array(argv);
// }

void exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec)
{
    char *cmd_path;
    char **argv;

    cmd_path = NULL;
    argv = cmd_prep(tokens, envp, &cmd_path, env);
    if (!argv)
    {
        g_exit_status = 127;
        return;
    }

    if (!apply_redirection(exec)) // Ensure redirection is successful
    {
        free(cmd_path);
        free_array(argv);
        g_exit_status = 1;
        return;
    }

    run_cmd(cmd_path, argv, envp);
    printf("Command executed successfully\n");

    free(cmd_path);
    free_array(argv);
}
