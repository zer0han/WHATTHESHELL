/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:25:56 by rdalal            #+#    #+#             */
/*   Updated: 2024/09/25 20:32:19 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_charprint(int c)
{
	write(1, &c, 1);
	return (1);
}

static int	ft_format(char type, va_list args)
{
	if (type == '%')
		return (write(1, "%", 1));
	else if (type == 'c')
		return (ft_printchar_fd(va_arg(args, int), 1), 1);
	else if (type == 's')
		return (ft_print_str(va_arg(args, char *)));
	else if (type == 'p')
		return (ft_print_ptr(va_arg(args, unsigned long)));
	else if (type == 'd' || type == 'i')
		return (ft_print_int(va_arg(args, int)));
	else if (type == 'u')
		return (ft_print_unsigned(va_arg(args, int)));
	else if (type == 'x')
		return (ft_print_hexa(va_arg(args, unsigned int), 0));
	else if (type == 'X')
		return (ft_print_hexa(va_arg(args, unsigned int), 1));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		len;

	va_start(args, str);
	i = 0;
	len = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '%')
		{
			len += ft_format(str[++i], args);
		}
		else
			len += ft_charprint(str[i]);
		i++;
	}
	va_end(args);
	return (len);
}
