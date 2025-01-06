/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/06 16:41:34 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/resource.h>
# include <signal.h>
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"
/* define identifiers stuff here
# define ...
*/

/*structs here*/

/*typedef struct s_data
{
	
}*/

typedef struct s_node
{
	char	*input;
	char	*data;
	int		value;
	char	*type;
	char	*left;
	char	*right;
}			t_node;

/*functions here*/


/***builtins***/
int	cmd_cd(char **agrv);

int	cmd_pwd(char **argv);

int	cmd_echo(char **argv);

int	cmd_export(char **argv, char **envp);

int	cmd_unset(char **argv, char **envp);

int	cmd_env(char **argv, char **envp);

int	cmd_exit(char **argv);

#endif