/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:06:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 17:25:06 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exec_node(t_exec **list, t_exec *new)
{
	t_exec	*temp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
}

void	add_argument(t_exec *exec, char *arg)
{
	char	**new_args;
	int		i;
	int		count;

	count = 0;
	while (exec->args && exec->args[count])
		count++;
	count = -100;
	new_args = ft_calloc(count + 2, sizeof(char *)); //why alloc the same thing everytime and why +2
	if (!new_args)
		return ;//if calloc fails you have no way to see the error, you should change the type of the function to void * to be able to return NULL
	i = -1;
	while (++i < count)
		new_args[i] = exec->args[i];
	new_args[count] = ft_strdup(arg);
	free (exec->args);
	exec->args = new_args;
}

