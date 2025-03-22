/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/22 19:05:07 by rdalal           ###   ########.fr       */
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

extern volatile sig_atomic_t	g_exit_status;

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

typedef struct s_envp
{
	char			*str;
	struct s_envp	*next;
	struct s_envp	*prev;
}					t_envp;

typedef struct s_token
{
	char			*input;
	char			*type;
	int				*value;
	struct s_token	*left;
	struct s_token	*right;
}					t_token;

typedef enum e_redir_type
{
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	char			*delimiter;
	//t_token			*redir;
	struct s_redir	*next;
	struct s_redir	*prev;
}					t_redir;

typedef struct s_exec
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
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
t_exec	*main_execution(t_token **token_tree, char **envp, t_envp *env_list);
/**exec_helper**/
void	add_exec_node(t_exec **list, t_exec *new);
//int		add_argument(t_exec *exec, char *arg);
int		count_args(t_token *node);
/*	helper				*/
void	sort_export_env(char **object);
int		exitcode_check(char *code);
int		valid_id(char *var);
// int		update_env(char ***envp, char *var, char *value);
// int		add_env(char ***envp, char *var, char *value);

/*	builtins			*/
//char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path);
void	run_cmd(char *cmd_path, char **argv, char **envp);
void	exec_external(t_token *tokens, char **envp, t_envp *env, t_exec *exec);
void	execute_cmds(t_token *token, char **envp, t_envp *env, t_exec *exec_list);
void	dispatch_cmds(t_token *tokens, t_envp *env, t_exec *exec_list);
int		fd_is_builtin(t_token *token);
int		cmd_cd(t_token *args);
int		cmd_pwd(void);
int		cmd_echo(t_token *tokens);
int		cmd_unset(t_envp *env, t_token *tokens);
int		cmd_env(t_envp *env);
int		cmd_exit(t_token *args, t_exec *exec_list, t_envp *env);

/*builtin_export.c*/
int		cmd_export(t_envp *env, t_token **tokens);
t_envp	*envp_dup(char **envp);
void	print_env(t_envp **dup);

/*  pipes               */
/**pipe_helper**/
void	handle_pipe_redir(t_exec *exec);
void	apply_redir_pipe(t_exec *exec);
void	child_process(t_exec *exec, char **envp, t_envp *env);
/**pipe**/
void	setup_child_process(t_exec *exec, char **envp, t_envp *env);
void	wait_for_children(t_exec *exec);
void	exec_pipeline(t_exec *exec, char **envp, t_envp *env);

/*  redirection         */
/**heredoc**/

/**redir_handler**/

/**redirection_exec**/

/**redirection**/
int		apply_redirection(t_exec *exec);
void	cleanup_redirection(t_redir *redir);
t_redir	*init_redir(t_token **cmd_token);
void	setup_redir(t_exec *exec);


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
void	free_envp(t_envp *dup);
void	free_all(t_token *tokens, t_exec *exec_list);

/*	tools.c	*/
char	*get_path(char *cmd);
void	free_tab(char **tab, int code);
char	*ft_fstrjoin(char *s1, char *s2);
int		is_builtin(char *cmd);

/*	main.c	*/
t_token	*ft_minishell_parsing(char *input, t_envp *env);

/*************EXPANSION*************/

/*	variable_exp.c	*/
char	*get_var_name(char *input_i);
int		new_input_len(char *input, t_envp *env);
void	replace_var_by_value(char *input, char *ninput, int *i, int *j, t_envp *env);
char	*expand_variables(char *input, t_envp *env);

/*	 variable_exp_tools.c	*/
void	handle_quote_after_dollar(char *input, char *ninput, int *i, int *j);
void	copy_quote(char *input, char *ninput, int *i, int *j, t_envp *env);
void	copy_dquote(char *input, char *ninput, int *i, int *j, t_envp *env);
void	copy_squote(char *input, char *ninput, int *i, int *j);
char	*my_getenv(char *var_name, t_envp **env);

/*************SIGNALS*************/

/*	 signal_handler.c	*/
void	signals(void);

#endif