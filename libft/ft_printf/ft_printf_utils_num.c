/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_num.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:37:45 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/22 17:38:46 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_unsigned(unsigned int n)
{
	char	buffer[10];
	int		i;
	int		printed;

	printed = 0;
	i = 0;
	if (n == 0)
	{
		buffer[i++] = '0';
	}
	while (n > 0)
	{
		buffer[i++] = (n % 10) + '0';
		n /= 10;
	}
	printed += i;
	while (i > 0)
	{
		write(1, &buffer[--i], 1);
	}
	return (printed);
}

int	print_hex(unsigned long long num, char format)
{
	char	*hex_digits;
	char	buffer[17];
	int		i;
	int		printed;

	i = 0;
	printed = 0;
	if (format == 'x')
		hex_digits = "0123456789abcdef";
	else
		hex_digits = "0123456789ABCDEF";
	if (num == 0)
		buffer[i++] = '0';
	while (num > 0)
	{
		buffer[i++] = hex_digits[num % 16];
		num /= 16;
	}
	printed = i;
	while (i > 0)
		write(1, &buffer[--i], 1);
	return (printed);
}

int	print_pointer(void *ptr)
{
	int	printed;

	printed = 0;
	if (!ptr)
	{
		return (write(1, "(nil)", 5));
	}
	printed = write(1, "0x", 2);
	printed += print_hex((unsigned long long)ptr, 'x');
	return (printed);
}
