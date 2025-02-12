/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:52:03 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/13 19:18:34 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*last;
	unsigned char	uc;

	uc = (unsigned char)c;
	last = NULL;
	while (*s != '\0')
	{
		if (*s == uc)
			last = s;
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return ((char *)last);
}
