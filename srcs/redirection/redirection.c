/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/03/16 18:13:28 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* in parsing created a linked list of t_token
* for redirection tokens:
** token->input is one: '>' '<' '>>' '<<'
** token->type is redirection
** token->right is a pointer to another t_token holding the filename (in its input feild)
*/

/*
* Heredoc redirection: this is more complex
* a full implementation would read lines until a delimiter is found
* for simplicity, we will just print a message
* in the finished implimentation
** create a temp file
** read from the stdin until the delimiter is found
** then open that file for reading and dup2 it to STDIN
* expects heredoc_token->right->input to have the delimiter.
* reads input lines from the user until the delimiter is reached
* does not add lines to history
* writes the content to a temp file
* redirects STDIN to the temp file
*/

int apply_redirection(t_exec *exec, t_token *redir, t_token *file)
{
    if (!redir || !file || !file->input)
        return (handle_error("syntax error", EINVAL, &redir), 1);

    if (ft_strcmp(redir->input, ">") == 0)
        exec->fd_out = open(file->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (ft_strcmp(redir->input, ">>") == 0)
        exec->fd_out = open(file->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (ft_strcmp(redir->input, "<") == 0)
        exec->fd_in = open(file->input, O_RDONLY);  // FIXED: Correct FD
    else if (ft_strcmp(redir->input, "<<") == 0)
        exec->fd_in = handle_heredoc(redir, file);  // FIXED: Assign correct fd

    // If opening the file failed, print error and return failure
    if (exec->fd_in == -1 || exec->fd_out == -1)
    {
        handle_error(file->input, errno, &redir);
        return (1);
    }
    return (0);
}


// void	redirection_process(t_exec *exec, t_token *token)
// {
// 	t_token	*file;
// 	t_token	*current;

// 	current = token;
// 	while (current)
// 	{
// 		if (current->input && (ft_strcmp(current->input, ">") == 0 \
// 			|| ft_strcmp(current->input, ">>") == 0 \
// 			|| ft_strcmp(current->input, "<") == 0 \
// 			|| ft_strcmp(current->input, "<<") == 0))
// 		{
// 			file = current->right;
// 			if (!file || !file->input)
// 				return ;//(handle_error("syntax error", EINVAL, &token));
// 			if (apply_redirection(exec, current, file) != 0)
// 				return ;
// 			current = file->right;
// 		}
// 		else
// 			current = current->right;
// 	}
// }

void	redirection_process(t_exec *exec, t_token *token)
{
	t_token	*file;
	t_token	*current = token;

	while (current)
	{
		if (current->input && (ft_strcmp(current->input, ">") == 0 \
			|| ft_strcmp(current->input, ">>") == 0 \
			|| ft_strcmp(current->input, "<") == 0 \
			|| ft_strcmp(current->input, "<<") == 0))
		{
			file = current->right;
			if (!file || !file->input)
				return (handle_error("syntax error", EINVAL, &token));
			if (apply_redirection(exec, token->redir, file) != 0)
				return ;

			current = file->right;
		}
		else
			current = current->right;
	}
}

