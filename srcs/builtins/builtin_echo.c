/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:48:47 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/28 18:10:13 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/***echo***/
	/*
	[options] = various options available for modifying the behavior of the echo command
	[string] = it is the string that we we want to display

	echo [option] [string]
	** the echo command writes char strings to STDOUTPUT
	** str are separated by spaces, and a new-line char follows the str parameter specified
	** if no str parameter is specified, new line (\n) is displayed
	echo -n
	echo -nnnnnn
	**-n/-nnnnnn (followed only by char 'n')
	***valid option
	***remove \n
	**if not followed by non 'n' like -nP -nL -n-n
	***invalid
*/

/*
need to make a function to check the -n condition in echo
shouldn't take too long can fix it quickly later but for now it is ok
void	n_check(char **argv)
{
	int	j;

	j = 1;
	while (argv[1][j])
	{
		if (argv[1][j] != 'n')
			break;
		j++;
	}
}*/

/*int	cmd_echo(int argc, char **argv)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;
	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	while (i < argc)
	{
		ft_printf("%s", argv[i]);
		if (i < argc - 1)
			ft_printf(" ");
		i++;
	}
	if (new_line)
		ft_printf("\n");
	return (0);
}*/
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
