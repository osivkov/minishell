/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_num.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:34:32 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/22 17:34:55 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_unsigned_format(va_list args)
{
	return (print_unsigned(va_arg(args, unsigned int)));
}

int	handle_hex_format(va_list args, char format)
{
	return (print_hex(va_arg(args, unsigned int), format));
}

int	handle_pointer_format(va_list args)
{
	void	*ptr;

	ptr = (void *)va_arg(args, unsigned long long);
	return (print_pointer(ptr));
}
