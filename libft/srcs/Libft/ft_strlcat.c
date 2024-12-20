/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:52:17 by rdalal            #+#    #+#             */
/*   Updated: 2024/05/20 16:52:19 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	count_src;
	size_t	len_dest;
	size_t	len_src;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	count_src = 0;
	if (size <= len_dest)
		return (len_src + size);
	while (src[count_src] && (len_dest + count_src + 1) < size)
	{
		dest[len_dest + count_src] = src[count_src];
		count_src++;
	}
	dest[len_dest + count_src] = '\0';
	return (len_dest + len_src);
}
