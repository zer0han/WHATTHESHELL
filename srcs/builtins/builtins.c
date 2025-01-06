/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/06 17:23:44 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*just error and basic struct here for the builtins can expand more on them
also have to write a function for the executions of the builtins*/

int	cmd_cd(char **argv)
{
	if (!argv[1])
	{
		ft_printf(stderr, "cd: missing arguement\n");
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	cmd_pwd(char **argv)
{
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

int	cmd_echo(char **argv)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		new_line = 0;
		i = 2;
	}
	while (argv[i])
	{
		if ((i > 2) || (i == 2 && new_line))
		{
			ft_printf(" ");
		}
		ft_printf("%s", argv[i]);
		i++;
	}
	if (new_line)
		ft_printf("\n");
	return (0);
}

int	cmd_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1])
		status = ft_atoi(argv[1]);
	ft_printf("exit\n");
	exit(status);
}
