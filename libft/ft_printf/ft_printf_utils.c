/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:55:08 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/22 17:40:51 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_char(char c)
{
	write(1, &c, 1);
}

int	print_string(const char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (str[i])
	{
		write (1, &str[i], 1);
		i++;
	}
	return (i);
}

int	print_number(int n)
{
	char	buffer[12];
	int		i;
	long	num;
	int		printed;

	printed = 0;
	num = n;
	i = 0;
	if (num < 0)
	{
		write(1, "-", 1);
		num = -num;
		printed++;
	}
	if (num == 0)
		buffer[i++] = '0';
	while (num > 0)
	{
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	printed += i;
	while (i > 0)
		write(1, &buffer[--i], 1);
	return (printed);
}
