/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:27 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/12 16:57:19 by rdalal           ###   ########.fr       */
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

/***pwd***/
int	cmd_pwd(char **argv)
{
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
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
/***echo***/
int	cmd_echo(char **argv)
{
	/*
	[options] = various options available for modifying the behavior of the echo command
	[string] = it is the string that we we want to display

	echo [option] [string]
	** the echo command writes char strings to STDOUTPUT
	** str are separated by spaces, and a new-line char follows the str parameter specified
	** if no str parameter is specified, new line (\n) is displayed
	echo -n
	echo -nnnnnn
	**-n/-nnnnnn (followed only by char 'n)
	***valid option
	***remove \n
	**if not followed by non 'n' like -nP -nL -n-n
	***invalid
	*/
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

/***builtin exit***/
int	cmd_exit(t_token *)
{
	/* exit [N (exit_nbr)]
	check if there are more than one arguments and if the first argument is not a valid exit code
	print error if not the correct number of arguments and return with exit code 1
	set default exit code at 0
	check if the first argument is a valid exit code(should be a nbr)
	if valid convert the arg to an int
	if arg is invalid, call exit error function to handle
	set exit code to 2 to indicate an error
	close any open resources in the shell context
	free memory in the shell context
	exit the shell with the determined exit code
	*/

	/*
	will have to make another function to check if the provided value can be a valid exit code
	and ensure the value is a nbr and is in the valid range of the exit codes.
	can be something like 
	int exitcode_check(char *value)
	*/
}

int	exitcode_check(char *value)
{
	/*
	allocated the value to a temp variable
	skip the signs
	iterate through each char in the value to check if it is a nbr
	if all chars are not nbr return 1 indicating invalid exit code
	if valid value: convert the value to a long int with ft_atol in libft
	check for over and underflows in the range of exit codes
	if not in range return 1 to indicate invalid exit code
	return 0 if the value is a valid exit code (no over and underflows, no non-nbr chars)
	the range is between 0 and 255 for the exit codes
	*/
}



/***cmd_export
*export
**to view all the exported variables.

*export -p
**to view all the exported variables on current shell

*export -f [function name]
**to export shell function
*Must be used if the names refer to functions.
*If -f is not used, the export will assume the names are variables.

*export -n [variable_name]
**Named variables (or functions, with -f) will no longer be exported
**No output will be seen on screen, to see exported variable grep from exported ones is used

*export without argument, the environment variables are displayed in alphabetical order.
**each variable is printed in the format
** declare -x VAR="value"
**sorting the environment ensures consistent behavior when user calls export

*export with arguments
**if args passed like export VAR=value, the variable is added or updated in the environment list without
needing sorting immediately

output

declare -x HOME="/home/user"
declare -x PATH="/usr/bin"
declare -x SHELL="/bin/bash"

***/

/***unset***/
 /* unset [variable_name]
 * unset [-options] [variable_name]
 ** undefine a variable in bash
 * unset -f [function_name]
 ** undefine shell function in bash
 */

/***env***/
int	cmd_env(char **argv, char **envp)
{
/*
*if no flags or parameters specified, the env command displays your current environment, showing one Name=Value per line
**Print out a list of all the env variables

*'-i' or '-ignore-environment' or '-'
**runs a command with an empty environment

[-i | -] [Name=Value]... [Command [Argument ...]]
*/	
}

