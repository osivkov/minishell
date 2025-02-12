/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:44:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/01/27 15:42:01 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_format(const char *format, va_list args)
{
	if (*format == 'c')
	{
		return (handle_char_format(args));
	}
	else if (*format == 's')
		return (handle_string_format(args));
	else if (*format == 'd' || *format == 'i')
		return (handle_integer_format(args));
	else if (*format == 'u')
		return (handle_unsigned_format(args));
	else if (*format == 'x' || *format == 'X')
		return (handle_hex_format(args, *format));
	else if (*format == 'p')
		return (handle_pointer_format(args));
	else if (*format == '%')
		return (write(1, "%", 1));
	return (0);
}

static int	handle_percent(const char **format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	if (**format == '%' && *(*format + 1) == '%')
	{
		write(1, "%", 1);
		printed_chars++;
		*format += 2;
	}
	else if (**format == '%' && *(*format + 1))
	{
		(*format)++;
		printed_chars += handle_format(*format, args);
		(*format)++;
	}
	else if (**format == '%' && !*(*format + 1))
	{
		write(1, "%", 1);
		printed_chars++;
		(*format)++;
	}
	return (printed_chars);
}

static int	ft_printf_core(const char *format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	while (*format)
	{
		if (*format == '%')
			printed_chars += handle_percent(&format, args);
		else
		{
			write(1, format, 1);
			printed_chars++;
			format++;
		}
	}
	return (printed_chars);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		printed_chars;

	va_start(args, format);
	printed_chars = ft_printf_core(format, args);
	va_end(args);
	return (printed_chars);
}
