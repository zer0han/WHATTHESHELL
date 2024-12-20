/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:53:20 by rdalal            #+#    #+#             */
/*   Updated: 2024/05/20 16:53:22 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *str, int c)
{
	size_t	i;
	char	*last_occ;

	i = 0;
	last_occ = NULL;
	while (str[i] != '\0')
	{
		if (str[i] == (char)c)
			last_occ = (char *)&str[i];
		i++;
	}
	if (str [i] == (char)c)
		last_occ = (char *)&str[i];
	return (last_occ);
}
