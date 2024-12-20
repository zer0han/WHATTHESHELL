/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hexa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:04:07 by rdalal            #+#    #+#             */
/*   Updated: 2024/06/06 15:04:08 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_putnbr_base(unsigned int nbr, const char *base)
{
	if (nbr >= 16)
		ft_putnbr_base(nbr / 16, base);
	write(1, &base[nbr % 16], 1);
}

static int	ft_numlen_base(unsigned int nbr, int base)
{
	int	len;

	len = 1;
	while (nbr >= (unsigned int)base)
	{
		nbr /= base;
		len++;
	}
	return (len);
}

int	ft_print_hexa(unsigned int nbr, int uppercase)
{
	const char	*base;

	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	ft_putnbr_base(nbr, base);
	return (ft_numlen_base(nbr, 16));
}
