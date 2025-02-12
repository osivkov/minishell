/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:19:57 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/14 12:05:26 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char			*d;
	const unsigned char		*s;
	size_t					i;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	if (dest == NULL && src == NULL)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

/*
#include <stdio.h>
#include <string.h>
void test_memcpy(void)
{
    char src[20] = "Hello, world!";
    char dest[20];

    printf("Testing ft_memcpy:\n");
    ft_memcpy(dest, src, 13);  // Копируем 13 байт (строку "Hello, world!")
    printf("Source: %s\n", src);
    printf("Destination after ft_memcpy: %s\n", dest);

    if (strcmp(dest, src) == 0)
    {
        printf("SUCCESS: The result is correct!\n\n");
    }
    else
    {
        printf("FAIL: The result is incorrect!\n\n");
    }
}

int main()
{
	test_memcpy();
}
*/