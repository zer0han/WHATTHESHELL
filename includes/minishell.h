/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/21 19:11:02 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <strings.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/resource.h>
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
	struct s_data	*cmd;
}					t_data;

typedef struct s_token
{
	char			*input;
	char			*type;
	int				*value;
	struct s_token	*left;
	struct s_token	*right;
	struct s_token	*redir;
}					t_token;

typedef struct s_exec
{
	char			*cmd;
	t_data			*args;
	t_token			*redir;
	int				fd_in;
	int				fd_out;
	int				fd_pipe[2];
	int				pid;
	struct s_exec	*next;
}					t_exec;

/*functions here*/


/*************EXECUTION*************/

/*  exec_functions  */
t_exec	*create_exec(t_token *cmd_token);
t_exec	*create_exec_list(t_token *token_tree);
void	main_execution(t_token *token_tree, t_data *code, char ***envp);

/*	helper		    */
void	sort_export_env(char **object);
void	free_shell(t_token *cmd_line);
void	free_array(char **args);
void	free_errors(t_token *cmd_line);
int		exitcode_check(char *code);
int		valid_id(char *var);
int		update_env(char **envp, char *var, char *value);
int		add_env(char ***envp, char *var, char *value);

/*  external_cmds       */

char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path);
void	run_cmd(char *cmd_path, char **argv, char **envp);
void	exec_external(t_token *tokens, char **envp);
void	dispatch_cmds(t_token *tokens, t_data *code, char ***envp);


/*	builtins	        */
int		cmd_cd(t_token *args);
int		cmd_pwd(t_token *args);
int		cmd_echo(t_token *tokens);
int		cmd_export(char ***envp, char **args);
int		cmd_unset(char **envp, char *var);
int		cmd_env(t_token *args, char **envp);
int		cmd_exit(t_data *code, t_token *args);

/*  pipes               */

int		make_pipe(int m_pipe[2]);
void	child_process1(t_exec *exec, int prev_pipe, char ***envp);
void	child_process2(t_exec *exec, int prev_pipe, int m_pipe[2], char ***envp);
void	exec_pipeline(t_exec *exec, char ***envp);

/*  redirection         */

void	heredoc_redirection(t_token *hd_token);
void	redirection_process(t_token *tokens);

/*************PARSING*************/

/*	tokenize_tools1.c	*/
void	alloc_fail(char **result, int iword);
char	*ft_strncdup(char const *src);
int		ft_count_tokens(char *line);
int		ft_search_unquote(char *line, char quote);
int		is_space(char s);

/*	tokenize_tools2.c	*/
char	*ft_strnqdup(char *src, char quote);
char	**ft_split_for_tokens(char *line);
t_token	*ft_lastnode(t_token *tokens);

/*	tokenize_tools3.c	*/
int		is_quote(char c);
t_token	*is_special_str(t_token **tokens, char *str);
void	*assign_token_type(t_token **tokens);
void	*assign_token_type2(t_token **tokens, t_token *node);
void	*assign_token_type3(t_token **tokens, t_token *node);

/*	tokenize_tools4.c	*/
void	assign_missing_cmds(t_token **tokens);
void	assign_options_and_args(t_token **tokens);
void	parse_after_cmds(t_token **node);
void	*parse_tokens(t_token **tokens);
void	*is_null_token(t_token **tokens);

/*	tokenize_tools5.c	*/
char	*assign_nosep_token(char *input, int *i);
int		is_nosep_token(char *input, int *i);

/*	tokenize.c	*/
t_token	*create_node(t_token **tokens, char *char_token);
t_token	*ft_tokenize(char *input);

/*	free.c	*/
void	free_string_tab(char **str_tab);
void	free_tokens(t_token *tokens);

/*	tools.c	*/
char	*get_path(char *cmd);
void	free_tab(char **tab, int code);
char	*ft_fstrjoin(char *s1, char *s2);
int		is_builtin(char *cmd);

/*************EXPANSION*************/

/*	variable_exp.c	*/
char	*get_var_name(char *input_i);
int		new_input_len(char *input);
void	replace_var_by_value(char *input, char *ninput, int *i, int *j);
char	*expand_variables(char *input);

/*	 variable_exp_tools.c	*/
void	handle_quote_after_dollar(char *input, char *ninput, int *i, int *j);
void	copy_quote(char *input, char *ninput, int *i, int *j);
void	copy_dquote(char *input, char *ninput, int *i, int *j);
void	copy_squote(char *input, char *ninput, int *i, int *j);

#endif