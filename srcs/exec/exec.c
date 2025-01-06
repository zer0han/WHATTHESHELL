/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:35:01 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/06 16:41:34 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*just pseudo code examples of how the exec funcitons will look*/

int	eval(t_node node)
{
	if (node.type == PLUS)
		return (node.left + node.right);
	else
		return (node.value);
}

int	execute(t_node node)
{
	if (node.type == PIPE)
		return (execute_pipe(node.left, node.right));
	else
		return (execute_simple_cmd(node.value));
}

int	execute_pipe(void)
{
	pipe(pipe_fds);
	left_pid = fork();
	if (pid == 0) //child0
	{
		// do stuff
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	else //back to the parent
	{
		// do the same thing for child1
		//make sure to close the pipe_fds in the parent too
	}
}

/*basically open a file and redirect using dup2 the STDIN or STDOUT to that file
*and in case of heredoc we redirect the STDIN to a pipe (|)
*/