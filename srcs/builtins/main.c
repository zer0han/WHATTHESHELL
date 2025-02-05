/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:41:24 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/05 16:58:44 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <strings.h>
#include <string.h>
#include <limits.h>
/*#include "minishell.h"*/
typedef struct s_data
{
	char			*args;
	int				nbr;
	struct s_data	*cmd;	
}					t_data;
typedef struct s_token 
{
	char			*input;
	char			*type;
	int				*value;
	struct s_token	*left;
	struct s_token	*right; 
}					t_token;




static char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(char *s)
{
	char	*dest;

	dest = malloc(strlen(s) + 1);
	if (!dest)
		return (NULL);
	ft_strcpy(dest, s);
	return (dest);
}

/**pwd***/
int	cmd_pwd(t_token **args)
{
	(void)args;
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

/***env***/
void	sort_export_env(char **object)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (object[i] && object [i + 1])
	{
		j = i + 1;
		if(strcmp(object[i], object[j]) > 0)
		{
			temp = object[i];
			object[i] = object[j];
			object[j] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int	cmd_env(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		sort_export_env(envp);
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

/***echo***/
int	cmd_echo(int argc, char **argv)
{
	int	i;
	int	nl;
	int	j;

	i = 1;
	nl = 1;
	j = 1;
	if (argc > 1 && argv[1][0] == '-')
	{
		while (argv[1][j] && argv[1][j] == 'n')
			j++;
		if (argv[1][j] == '\0')
		{
			nl = 0;
			i++;
		}
	}
	if (i < argc)
		nl = 1;
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


/***cd***/
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

/***exit***/
void	free_shell(t_token *cmd_line)
{
	t_token	*temp;
	t_token	*current;

	if (!cmd_line)
		return ;
	current = cmd_line;
	while (current)
	{
		temp = current->right;
		current->value = 0;
		free(current);
		current = temp;
	}
	cmd_line = NULL;
}

void	free_errors(t_token *cmd_line)
{
	if (cmd_line && cmd_line->right)
		free_shell(cmd_line);
	printf("error\n");
	exit (1);
}

int	exitcode_check(char *code)
{
	char	*temp;
	char	nbr;

	temp = code;
	if ((*temp == '+' || *temp == '-') && *(temp + 1))
		temp++;
	while (*temp)
	{
		if (!isdigit(*temp++))
			return (1);
	}
	nbr = atol(code);
	if ((nbr > 0 && (LONG_MAX / nbr < 1)) || \
		(nbr < 0 && (LONG_MIN / atol(code) < 1)))
		return (1);
	return (0);
}

int	cmd_exit(t_data *code, t_token *args)
{
	int	exit_code;

	if (args && args->right && !exitcode_check(args->input))
		return(printf("minishell: exit: too many arguments\n"), 1);
	exit_code = 0;
	if (args && !exitcode_check(args->input))
		exit_code = atoi(args->input);
	else if (args && exitcode_check(args->input))
	{
		exitcode_check (args->input);
		exit_code = 2;
	}
	close(code->nbr);
	free_shell(args);
	exit(exit_code);
}

/****EXPORT******/
int	valid_id(char *var)
{
	if (!var || !((*var >= 'A' && *var <= 'Z') || \
		(*var >= 'a' && *var <= 'z')))
		return (0);
	var++;
	while (*var)
	{
		if (!((*var >= 'A' && *var <= 'Z') || (*var >= 'a' && *var <= 'z')) \
			|| (*var >= '0' && *var <= '9') || (*var == '_'))
			return (0);
		var++;
	}
	return (1);
}

int	update_env(char **envp, char *var, char *value)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], var, strlen(var)) == 0 && \
			envp[i][strlen(var)] == '=')
		{
			//free (envp[i]);
			envp[i] = malloc(strlen(var) + strlen(value) + 2);
			if (!envp[i])
				return (1);
			sprintf(envp[i], "%s=%s", var, value);
			return (0);
		}
		i++;
	}
	return (1);
}

int	add_env(char ***envp, char *var, char *value)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while ((*envp)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[count] = malloc(strlen(var) + strlen(value) + 2);
	if (!new_env[count])
		return (1);
	sprintf(new_env[count], "%s=%s", var, value);
	new_env[count + 1] = NULL;
	//free(*envp);
	*envp = new_env;
	return (0);
}

int	cmd_export(char ***envp, char **args)
{
	int		i;
	int		j;
	char	*e_s;
	char	*var;
	char	*value;

	i = 1;
	if (!args[1])
	{
		j = 0;
		while ((*envp)[j])
		{
			sort_export_env(*envp);
			printf("export %s\n", (*envp)[j++]);
		}
		return (0);
	}
	while (args[i])
	{
		e_s = strchr(args[i], '=');
		if (e_s)
		{
			*e_s = '\0'; //to split the var and the value (var=value)
			var = args[i];
			value = e_s + 1;
			if (!valid_id(var))
				return (printf("export: not a valid argument1\n"), 1);
			if (update_env(*envp, var, value))
				add_env(envp, var, value);
		}
		else 
		{
			if (!valid_id(args[i]))
				return (printf("export: not a valid argument2\n"), 1);
		}
		i++;
	}
	return (0);
}

/******UNSET*****/
int	cmd_unset(char **envp, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(var);
	if (!var)
		return (1);
	if (!valid_id(var))
		return(1);
	while (envp[i])
	{
		if (strncmp(envp[i], var, len) == 0 && (envp[i][len] == '=' \
			|| envp[i][len] == '\0'))
		{
			printf("match found: \"%s\" for var \"%s\"\n", envp[i], var); //debug check
			free(envp[i]);
			while (envp[i])
			{
				envp[i] = envp[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	printf("var \"%s\" not found in envp. \n", var); //debug check
	return (0);
}

int	main(int argc, char **argv, char **envp) 
{
	char	*input;
	char	*token_argv[1024];
	int		token_argc;
	int		i = 1;
	t_token	*new_token;
	t_token	*args = NULL;
	t_token	*current = NULL;
	t_data	ctx;
	ctx.cmd = NULL;
	ctx.args = NULL;
	t_token	*temp;

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
		while (i < token_argc)
		{
			new_token = malloc(sizeof(t_token));
			new_token->input = ft_strdup(token_argv[i]);
			new_token->right = NULL;
			if (!args)
				args = new_token;
			current = new_token;
			i++;
		}
		if (strcmp(input, "pwd") == 0)
			cmd_pwd(NULL);
		else if (strcmp(input, "cd") == 0)
			cmd_cd(token_argv);
		else if (strcmp(input, "echo") == 0)
			cmd_echo(token_argc, token_argv);
		else if (strcmp(input, "env") == 0)
			cmd_env(envp);
		else if (strcmp(input, "exit") == 0)
			cmd_exit(&ctx, args);
		else if (strcmp(input, "export") == 0)
			cmd_export(&envp, token_argv);
		else if (strcmp(input, "unset") == 0)
		{
			if (token_argc > 1)
				cmd_unset(envp, token_argv[1]);
			else
				printf("unset: missing arg\n");
		}
		else
			printf("Input entered: %s\n", token_argv);
		while (args)
		{
			temp = args->right;
			free(args->input);
			free(args->value);
			free(args);
			args = temp;
		}
		free (input);
	}
	printf("Exiting whattheshell\n");
	return (0);
}


/*void	cmd_line(int token_ac, char *input, char *token_av[1024], char **envp)
{
	
		token_ac = 0;
		token_av[token_ac] = strtok(input, " ");
		if (strcmp(input, "pwd") == 0)
			cmd_pwd(NULL);
		else if (strcmp(input, "cd") == 0)
			cmd_cd(token_av);
		else if (strcmp(input, "echo") == 0)
			cmd_echo(token_ac, token_av);
		else if (strcmp(input, "env") == 0)
			cmd_env(token_av, envp);
		else if (strcmp(input, "exit") == 0)
			cmd_exit();
		else
			printf("Input entered: %s\n", input);
		free (input);
}*/

