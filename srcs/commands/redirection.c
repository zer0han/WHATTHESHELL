/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:30:19 by rdalal            #+#    #+#             */
/*   Updated: 2025/02/05 15:41:00 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	redirection_process(t_token *tokens)
{
	t_token	*current;
	int		fd;

	current = tokens;
	while (current)
	{
		
	}
	/*do redirection after forking, do forking before piping*/
}