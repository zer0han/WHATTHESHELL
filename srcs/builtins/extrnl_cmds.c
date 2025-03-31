/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrnl_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:31:21 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/31 18:06:05 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*call your the path functions here */

// static int	ft_count_args(t_token *tokens)
// {
// 	int		count;
// 	t_token	*temp;

// 	count = 0;
// 	temp = tokens;
// 	while (temp  && ((ft_strcmp(temp->type, "arg") == 0) || \
// 	ft_strcmp(temp->type, "option") == 0 || ft_strcmp(temp->type, "cmd") == 0))
// 	{
// 		count++;
// 		temp = temp->right;
// 	}
// 	return (count);
// }

// static char	**fill_argv(t_token *tokens, int argc, t_envp *env)
// {
// 	char	**argv;
// 	int		i;

// 	argv = malloc(sizeof(char *) * (argc + 1));
// 	if (!argv)
// 		return (NULL);
// 	i = 0;
// 	while (i < argc && tokens)
// 	{
// 		argv[i] = expand_variables(tokens->input, env);
// 		if (!argv[i])
// 		{
// 			free_array(argv);
// 			return (NULL);
// 		}
// 		tokens = tokens->right;
// 		i++;
// 	}
// 	argv[argc] = NULL;
// 	return (argv);
// }

// static char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path, t_envp *env)
// {
// 	char	**argv;
// 	int		argc;

// 	(void)envp;
// 	argc = ft_count_args(tokens);
// 	argv = fill_argv(tokens, argc, env);
// 	if (!argv)
// 		return (NULL);
// 	*cmd_path = get_path(argv[0]);
// 	if (!*cmd_path)
// 	{
// 		ft_putstr_fd("WHATTHESHELL: cmd not found\n", STDERR_FILENO);
// 		free_array(argv);
// 		return (NULL);
// 	}
// 	return (argv);
// }

// void	run_cmd(char *cmd_path, char **argv, char **envp)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		free(cmd_path);
// 		return (free_array(argv));
// 	}
// 	else if (pid > 0)
// 	{
// 		execve(cmd_path, argv, envp);
// 		ft_putstr_fd("WHATTHESHELL: \n", STDERR_FILENO);	
// 		free(cmd_path);
// 		free_array(argv);
// 		exit(127);
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			g_exit_status = WEXITSTATUS(status);
// 	}
// }

// void exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec)
// {
//     char *cmd_path;
//     char **argv;

//     cmd_path = NULL;
//     argv = cmd_prep(tokens, envp, &cmd_path, env);
//     if (!argv)
//     {
//         g_exit_status = 127;
//         return;
//     }

//     if (!apply_redirection(exec)) // Ensure redirection is successful
//     {
//         free(cmd_path);
//         free_array(argv);
//         g_exit_status = 1;
//         return;
//     }

//     run_cmd(cmd_path, argv, envp);
//     printf("Command executed successfully\n");

//     free(cmd_path);
//     free_array(argv);
// }


// void exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec)
// {
// 	(void)exec;
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

char	**env_to_array (t_envp *env)
{
	t_envp	*temp;
	char	**array;
	int		count;
	int		i;

	count = 0;
	temp = env;
	while(temp && ++count)
		temp = temp->next;
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	temp = env;
	while (temp)
	{
		array[i] = ft_strdup(temp->str);
		if (!array[i])
		{
			while (--i >= 0)
				free (array[i]);
			free (array);
			return (NULL);
		}
		i++;
		temp = temp->next;
	}
	array[i] = NULL;
	return (array);
}

void	exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec)
{
	char	*path;
	char	**argv;
	pid_t	pid;
	int		status;

	(void)tokens;
	(void)envp;
	path = get_path(exec->cmd);
	if (!path)
	{
		error_message(exec->cmd, errno);
		g_exit_status = 127;
		return ;
	}
	argv = exec->args;
	if (exec->is_pipeline)
	{
		execve(path, argv, env_to_array(env));
		error_message("execve", errno);
		exit(126);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			printf("fcntl(0) = %d\n", fcntl(0, F_GETFD));
			execve(path, argv, env_to_array(env));
			error_message("execve", errno);
			exit(126);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			g_exit_status = WEXITSTATUS(status);
		}
	}
	free (path);
}
