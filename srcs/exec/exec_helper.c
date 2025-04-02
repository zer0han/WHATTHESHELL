/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:06:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/04/02 16:24:12 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exec_node(t_exec **list, t_exec *new)
{
	t_exec	*temp;

	if (!new)
		return ;
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

int	count_args(t_token *node)
{
	int	count;

	count = 0;
	while (node && (ft_strcmp(node->type, "arg") == 0 || \
		ft_strcmp(node->type, "option") == 0))
	{
		count++;
		node = node->right;
	}
	return (count);
}
