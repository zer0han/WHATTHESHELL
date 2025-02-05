/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/05 19:32:19 by rdalal           ###   ########.fr       */
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

void	redirection_process(t_token *tokens)
{
	t_token	*current;
	int		fd;

	current = tokens;
	while (current)
	{
		if (current->type && ft_strcmp(current->type, "redirection") == 0)
		{
			if (ft_strcmp(current->input, ">") == 0)
			{
				//output redirection: turncate (or create)
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd = -1)
					{
						perror("open");
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					closee (fd);
				}
			}
			else if (ft_strcmp(current->input, ">>") == 0)
			{
			// append output redirection
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd == -1)
					{
						perror("open");
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
			}
			else if(ft_strcmp(current->input, "<") == 0)
			{
				//input redirection: open file for reading
				if (current->right && current->right->input)
				{
					fd = open(current->right->input, O_RDONLY);
					if (fd == -1)
					{
						perror("open");
						exit (1);
					}
					dup2(fd, STDIN_FILENO);
					close (fd);
				}
			}
			else if (ft_strcmp(current->input, "<<") == 0)
			{
				/*
				* Heredoc redirection: this is more complex
				* a full implementation would read lines until a delimiter is found
				* for simplicity, we will just print a message
				* in the finished implimentation
				** create a temp file
				** read fromt stdin until the delimiter is found
				** then open that file for reading and dup2 it to STDIN
				*/
				printf ("Heredoc redirection is not yet finished\n");
			}
		}
		current = current->right;
	}
}
