/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:23 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 10:20:10 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	strlen_alt(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

//frees all string
void	ft_free_single(char *s1, char *s2, char *s3, char *s4)
{
	if (s1 != NULL)
	{
		free (s1);
		s1 = NULL;
	}
	if (s2 != NULL)
	{
		free (s2);
		s2 = NULL;
	}
	if (s3 != NULL)
	{
		free (s3);
		s3 = NULL;
	}
	if (s4 != NULL)
	{
		free (s4);
		s4 = NULL;
	}
	return ;
}

//frees all 2D char array
void	ft_free_double(char **s1, char **s2, char **s3, char **s4)
{
	size_t	i;

	if (s1 != NULL)
	{
		i = -1;
		while (s1[++i] != NULL)
		{
			free (s1[i]);
			s1[i] = NULL;
		}
		free (s1);
		s1 = NULL;
	}
	if (s2 != NULL)
		ft_free_double(s2, NULL, NULL, NULL);
	if (s3 != NULL)
		ft_free_double(s3, NULL, NULL, NULL);
	if (s4 != NULL)
		ft_free_double(s4, NULL, NULL, NULL);
	return ;
}

//joins all strings. returns NULL in case of malloc failure
char	*ft_strjoin_all(const char *s1, const char *s2, const char *s3, \
	const char *s4)
{
	size_t	len;
	size_t	i;
	char	*final;

	len = strlen_alt(s1) + strlen_alt(s2) + strlen_alt(s3) + strlen_alt(s4);
	final = (char *)ft_calloc(sizeof(char), (len + 1));
	if (final == NULL)
		return (NULL);
	i = -1;
	len = -1;
	while (s1 && s1[++i] != '\0')
		final[++len] = s1[i];
	i = -1;
	while (s2 && s2[++i] != '\0')
		final[++len] = s2[i];
	i = -1;
	while (s3 && s3[++i] != '\0')
		final[++len] = s3[i];
	i = -1;
	while (s4 && s4[++i] != '\0')
		final[++len] = s4[i];
	return (final);
}
