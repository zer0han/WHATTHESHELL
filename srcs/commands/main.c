/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:41:24 by rdalal            #+#    #+#             */
/*   Updated: 2024/12/18 19:03:03 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*something like this this is just a very basic example*/

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("megashell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		ft_printf("Input entered: %s\n", input);
		free (input);
	}
	ft_printf("Exiting megashell\n");
	return (0);
}
