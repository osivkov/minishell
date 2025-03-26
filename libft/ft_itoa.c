/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:25:31 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/25 09:35:03 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_len(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	num;
	int		len;
	char	*str;

	num = n;
	len = num_len(num);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num == 0)
	{
		return (str[0] = '0', str);
	}
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	while (num)
	{
		str[--len] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}
