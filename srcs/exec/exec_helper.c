/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:06:16 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/27 18:53:03 by rdalal           ###   ########.fr       */
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

/*int	add_argument(t_exec *exec, char *arg)
{
	char	**new_args;
	int		i;
	int		count;

	if (!exec || !arg)
		return (1);
	count = 0;
	while (exec->args && exec->args[count])
		count++;
	new_args = malloc((count + 2) * sizeof(char *)); //why alloc the same thing everytime and why 
		if (!new_args)
		return (1);//if calloc fails you have no way to see the error, you should change the type of the function to void * to be able to return NULL
	i = -1;
	while (++i < count)
		new_args[i] = exec->args[i];
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free (exec->args);
	exec->args = new_args;
	return (0);
}*/

int		count_args(t_token *node)
{
	int	count;

	count = 0;
	while (node && (ft_strcmp(node->type, "arg") == 0 || \
		ft_strcmp(node->type, "option") == 0))
	{
		count ++;
		node = node->right;	
	}
	return (count);
}
