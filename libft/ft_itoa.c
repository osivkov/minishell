/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:25:31 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/13 18:17:21 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long nb)
{
	int	len;

	if (nb <= 0)
		len = 1;
	else
		len = 0;
	while (nb)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nb;

	nb = n;
	len = count_digits(nb);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	nb = n;
	if (n < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	else if (n == 0)
		str[0] = '0';
	while (nb)
	{
		str[--len] = '0' + (nb % 10);
		nb /= 10;
	}
	return (str);
}
