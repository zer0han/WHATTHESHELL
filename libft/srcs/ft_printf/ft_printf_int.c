/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 15:42:10 by rdalal            #+#    #+#             */
/*   Updated: 2024/06/18 15:42:14 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	num_len(int n)
{
	int	len;

	len = 1;
	if (n < 1)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_itoa(int n)
{
	int				len;
	unsigned int	nbr;
	char			*result;

	len = num_len(n);
	if (n < 0)
		nbr = -n;
	else
		nbr = n;
	result = (char *)malloc(len * sizeof(char));
	if (!result)
		return (NULL);
	result [len - 1] = '\0';
	while (len > 1)
	{
		len--;
		result [len -1] = nbr % 10 + '0';
		nbr /= 10;
	}
	if (n < 0)
		result[0] = '-';
	return (result);
}

int	ft_print_int(int n)
{
	int		len;
	char	*nbr;

	nbr = ft_itoa(n);
	if (!nbr)
		return (0);
	len = ft_print_str(nbr);
	free (nbr);
	return (len);
}
