/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:08:44 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/02 17:22:40 by gmechaly         ###   ########.fr       */
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
	// int				*value;
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
	char			**delimiter;
	//t_token			*redir;
	struct s_redir	*next;
	struct s_redir	*prev;
}					t_redir;

typedef struct s_exec
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
	int				redir_applied;
	t_token			*cmd_token;
	int				fd_in;
	int				fd_out;
	int				fd_pipe[2];
	int				p_pipe;
	int				is_pipeline;
	int				pid;
	char			*heredoc_file;
	int				std_save[2];
	struct s_exec	*next;
	struct s_exec	*prev;
}					t_exec;

typedef struct s_vexp
{
	int		i;
	int		j;
	char	*o_ipt;
	char	*n_ipt;
}			t_vexp;


/*functions here*/
 /*TEMP*/
 void	ft_print_tokens(t_token **tokens);

/*************ERROR HANDLING********/

void	handle_error(char *context, int errnum, t_token **tokens);
int		error_message(char *context, int error_code);

/*************EXECUTION*************/

/*  exec_functions  */
t_exec	*create_exec(t_token *cmd_token);
t_exec	*create_exec_list(t_token *token_tree);
t_exec	*main_execution(t_token **token_tree, t_envp *env);
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
int		fd_is_builtin(t_token *token);
//char	**cmd_prep(t_token *tokens, char **envp, char **cmd_path);
// void	run_cmd(char *cmd_path, char **argv, char **envp);
void	exec_external(t_envp *env, t_exec *exec);
void	execute_cmds(t_token *token, t_envp *env, t_exec *exec_list);
void	dispatch_cmds(t_token *tokens, t_envp *env, t_exec *exec_list);
int		fd_is_builtin(t_token *token);
int		cmd_cd(t_token *args);
int		cmd_pwd(void);
int		cmd_echo(t_token *tokens);
int		cmd_unset(t_envp *env, t_token *tokens);
int		cmd_env(t_envp *env);
int		cmd_exit(t_token *args, t_exec *exec_list, t_envp *env);
char	**env_to_array (t_envp *env);

/*builtin_export.c*/
int		cmd_export(t_envp *env, t_token **tokens);
t_envp	*envp_dup(char **envp);
void	print_env(t_envp **dup);

/*  pipes               */
/**pipe_helper**/
void	handle_pipe_redir(t_exec *exec);
//void	apply_redir_pipe(t_exec *exec);
void	child_process(t_exec *exec, t_envp *env);
/**pipe**/
void	setup_child_process(t_exec *exec, t_envp *env);
void	wait_for_children(t_exec *exec);
void	exec_pipeline(t_exec *exec, t_envp *env);

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

/*	assign_tokens.c	*/
void	*assign_token_type(t_token **tokens);
void	assign_missing_cmds(t_token **tokens);
void	assign_options_and_args(t_token **tokens);

/*	custom_libft.c	*/
char	*ft_strncdup(char const *src);
char	*ft_strnqdup(char *src, char quote);
char	*ft_fstrjoin(char *s1, char *s2);

/*	parse_tokens.c	*/
void	parse_after_cmds(t_token **node);
void	*parse_tokens(t_token **tokens);
void	*is_null_token(t_token **tokens);

/*	split_for_tokens.c	*/
char	**ft_split_for_tokens(char *line);

/*	split_utils.c	*/
int	ft_search_unquote(const char *line, char quote);
int	is_nosep_token(char *input, int *i);

/*	struct_utils.c	*/
t_token	*create_node(t_token **tokens, char *char_token);
t_token	*ft_lastnode(t_token *tokens);
t_token	*is_special_str(t_token **tokens, char *str);

/*	tokenize.c	*/
t_token	*ft_tokenize(char *input);

/*	tools.c	*/
char	*get_path(char *cmd);
int		is_quote(char c);
int		is_space(char s);
void	alloc_fail(char **result, int iword);

/*	free_chars.c	*/
void	free_string_tab(char **str_tab);
void	free_array(char **args);

/*	free_structs.c	*/
void	free_tokens(t_token *tokens);
void	free_exec(t_exec *exec_list);
void	free_envp(t_envp *dup);
void	free_all(t_token *tokens, t_exec *exec_list);

/*	main.c	*/
t_token	*ft_minishell_parsing(char *input, t_envp *env);

/*************EXPANSION*************/

/*	var_tools.c	*/
int		get_var_len(char *input, int *i, int *add_len, t_envp *env);
char	*get_var_name(char *input_i);
void	replace_var_by_value(t_vexp *data, t_envp *env);

/*	variable_exp.c	*/
int		new_input_len(char *input, t_envp *env);
char	*expand_variables(char *input, t_envp *env);

/*	 variable_exp_tools.c	*/
void	handle_quote_after_dollar(t_vexp *data);
void	copy_quote(t_vexp *data, t_envp *env);
void	copy_dquote(t_vexp *data, t_envp *env);
void	copy_squote(t_vexp *data);
char	*my_getenv(char *var_name, t_envp **env);

/*************SIGNALS*************/

/*	 signal_handler.c	*/
void	signals(void);

#endif