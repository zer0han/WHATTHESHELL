/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:06:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/24 18:23:08 by rdalal           ###   ########.fr       */
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
	new_args = ft_calloc(i + 2, sizeof(char *));
	if (!new_args)
		return ;
	i = -1;
	while (++i < count)
		new_args[i] = exec->args[i];
	new_args[count] = ft_strdup(arg);
	free (exec->args);
	exec->args = new_args;
}

