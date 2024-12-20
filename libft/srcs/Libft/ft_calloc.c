/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 19:50:33 by rdalal            #+#    #+#             */
/*   Updated: 2024/09/25 19:27:34 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	size_t			size_total;
	unsigned char	*pntr;

	if (nmemb != 0 && size != 0 && ((size_t) - 1 / nmemb < size))
		return (NULL);
	size_total = nmemb * size;
	pntr = malloc(size_total);
	if (pntr == NULL)
		return (NULL);
	i = 0;
	while (i < size_total)
	{
		pntr[i] = 0;
		i++;
	}
	return (pntr);
}
