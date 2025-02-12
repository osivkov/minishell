/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:37:58 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/22 17:40:04 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *format, ...);
void	print_char(char c);
int		print_string(const char *str);
int		print_number(int n);
int		print_unsigned(unsigned int n);
int		print_hex(unsigned long long num, char format);
int		print_pointer(void *ptr);
int		handle_pointer_format(va_list args);
int		handle_hex_format(va_list args, char format);
int		handle_unsigned_format(va_list args);
int		handle_integer_format(va_list args);
int		handle_string_format(va_list args);
int		handle_char_format(va_list args);

#endif