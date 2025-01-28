/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/28 18:52:10 by gmechaly         ###   ########.fr       */
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
# include <limits.h>
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
// variable types are just placeholders for now

typedef struct s_data
{
	char			*args;
	int				nbr;
	struct s_data	cmd;	
}					t_data;

typedef struct s_token
{
	char			*input;
	char			*type;
	int				*value;
	struct s_token	*left;
	struct s_token	*right;
}					t_token;

/*functions here*/


/***builtins***/

//change the prototypes and use t_token node instead of char
int		cmd_cd(char **agrv);

int		cmd_pwd(char **argv);

int		cmd_echo(char **argv);

int		cmd_export(char **argv, char **envp);

int		cmd_unset(char **argv, char **envp);

int		cmd_env(char **argv, char **envp);

//int	cmd_exit(char **argv);

/*************PARSING*************/

/*	tokenize_tools1.c	*/
void	alloc_fail(char **result, int iword);
char	*ft_strncdup(char const *src);
int		ft_count_tokens(char *line);
int		ft_search_unquote(char *line, int *count, char quote);
int		is_space(char s);

/*	tokenize_tools2.c	*/
char	*ft_strnqdup(char *src, char quote);
char	**ft_split_for_tokens(char *line);
t_token	*ft_lastnode(t_token *tokens);

/*	tokenize_tools3.c	*/
t_token	*is_special_str(t_token **tokens, char *str);
void	*assign_token_type(t_token **tokens);
void	*assign_token_type2(t_token **tokens);
void	*assign_token_type3(t_token **tokens);

/*	tokenize_tools4.c	*/
void	assign_missing_cmds(t_token **tokens);
void	assign_options_and_args(t_token **tokens);
void	parse_after_cmds(t_token **node);

/*	tokenize.c	*/
t_token	*create_node(t_token **tokens, char *char_token);
t_token	*ft_tokenize(char *input);

/*	free.c	*/
void	free_string_tab(char **str_tab);

/*	tools.c	*/
char	*get_path(char *cmd);
void	free_tab(char **tab, int code);
char	*ft_fstrjoin(char *s1, char *s2);
int		is_builtin(char *cmd);

#endif