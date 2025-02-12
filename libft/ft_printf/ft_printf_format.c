/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:45:11 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/22 17:36:27 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_char_format(va_list args)
{
	print_char(va_arg(args, int));
	return (1);
}

int	handle_string_format(va_list args)
{
	return (print_string(va_arg(args, char *)));
}

int	handle_integer_format(va_list args)
{
	return (print_number(va_arg(args, int)));
}
