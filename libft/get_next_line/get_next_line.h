/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:04:38 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/14 15:44:22 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	*gnl_ft_strjoin(char const *s1, char const *s2);
char	*gnl_ft_strchr(const char *string, int searchedChar );
void	gnl_ft_bzero(void *s, size_t n);
void	*gnl_ft_calloc(size_t elementCount, size_t elementSize);
size_t	gnl_ft_strlen(const char *theString);

#endif
