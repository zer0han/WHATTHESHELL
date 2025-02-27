/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 13:01:14 by rdalal           ###   ########.fr       */
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
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/resource.h>
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"

extern int	g_exit_status;
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
	char			**args;
	t_token			*redir;
	t_token			*cmd_token;
	int				fd_in;
	int				fd_out;
	int				fd_pipe[2];
	int				p_pipe;
	int				pid;
	struct s_exec	*next;
	struct s_exec	*prev;
}					t_exec;

/*functions here*/


/*************ERROR HANDLING********/

void	free_array(char **args);
void	handle_error(char *context, int errnum, t_token **tokens);
int		error_message(char *context, int error_code);

/*************EXECUTION*************/

/*  exec_functions  */
t_exec	*create_exec(t_token *cmd_token);
t_exec	*create_exec_list(t_token *token_tree);
t_exec	*main_execution(t_token **token_tree, char **envp);
/**exec_helper**/
void	add_exec_node(t_exec **list, t_exec *new);
void	add_argument(t_exec *exec, char *arg);

/*	helper				*/
void	sort_export_env(char **object);
int		exitcode_check(char *code);
int		valid_id(char *var);
int		update_env(char **envp, char *var, char *value);
int		add_env(char ***envp, char *var, char *value);

/*	builtins			*/
char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path);
void	run_cmd(char *cmd_path, char **argv, char **envp);
void	exec_external(t_token *tokens, char **envp);
void	execute_cmds(t_token *token, char **envp);
int		dispatch_cmds(t_token *tokens, char ***envp);
int		cmd_cd(t_token *args);
int		cmd_pwd(t_token *args);
int		cmd_echo(t_token *tokens);
int		cmd_export(char ***envp, t_token *tokens);
int		cmd_unset(char ***envp, t_token *tokens);
int		cmd_env(t_token *args, char **envp);
int		cmd_exit(t_token *args);

/*  pipes               */
/**pipe_helper**/
void	handle_pipe_redir(t_exec *exec);
void	apply_redir(t_exec *exec);
void	child_process(t_exec *exec, char **envp);
/**pipe**/
void	setup_child_process(t_exec *exec, char **envp);
void	wait_for_children(t_exec *exec);
void	exec_pipeline(t_exec *exec, char **envp);

/*  redirection         */
/**heredoc**/
int		create_heredoc_file(char **temp);
int		handle_heredoc(t_token *redir, t_token *file);
void	read_heredoc_content(int fd, char *limit);
/**redir_handler**/
int		handle_output(t_token *redir, t_token *file, t_token **token);
int		handle_append(t_token *redir, t_token *file, t_token **token);
int		handle_input(t_token *redir, t_token *file, t_token **token);
/**redirection_exec**/
void	setup_redir(t_exec *exec);
void	clean_fds(t_exec *exec);
void	execute_child_redir(t_exec *exec, char **envp);
void	redir_execute_cmd(t_exec *exec, char **envp);
/**redirection**/
void	redirection_process(t_token *token);
int		apply_redirection(t_token *redir, t_token *file, t_token **token);


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
void	*ft_split_for_tokens_2(char *line, char **result, int *i, int *iword);
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
void	free_exec(t_exec *exec_list);
void	free_all(t_token *tokens, t_exec *exec_list);

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