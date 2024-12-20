/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:03:41 by rdalal            #+#    #+#             */
/*   Updated: 2024/06/06 15:03:43 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_nbr_len(unsigned int nbr)
{
	int	len;

	len = 1;
	while (nbr >= 10)
	{
		len++;
		nbr /= 10;
	}
	return (len);
}

static char	*ft_unsigned_itoa(unsigned int n)
{
	char	*nbr;
	int		len;

	len = ft_nbr_len(n);
	nbr = (char *)malloc(sizeof(char) * (len + 1));
	if (!nbr)
		return (0);
	nbr[len] = '\0';
	while (n != 0)
	{
		nbr[--len] = n % 10 + '0';
		n = n / 10;
	}
	return (nbr);
}

int	ft_print_unsigned(unsigned int n)
{
	char	*nbr;
	int		prt_len;

	prt_len = 0;
	if (n == 0)
		prt_len += write (1, "0", 1);
	else
	{
		nbr = ft_unsigned_itoa(n);
		prt_len += ft_print_str(nbr);
		free (nbr);
	}
	return (prt_len);
}
