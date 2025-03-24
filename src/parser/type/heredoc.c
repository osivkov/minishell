/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 14:41:57 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
/*
 * handle_heredoc:
 *  - Takes a delimiter string.
 *  - Creates a temporary file.
 *  - Reads lines from the user (using readline) until the input exactly matches the delimiter.
 *  - Writes each line (with a newline) into the temporary file.
 *  - Resets the file offset to the beginning.
 *  - Returns the file descriptor for the temporary file.
 */

//  static char	*sanitize_input(char *s)
//  {
// 	 size_t	i = 0, j = 0;
// 	 char	*clean;
 
// 	 if (!s)
// 		 return (NULL);
// 	 clean = malloc(ft_strlen(s) + 1);
// 	 if (!clean)
// 		 return (NULL);
// 	 while (s[i])
// 	 {
// 		 if ((unsigned char)s[i] == 27)  // ESC
// 		 {
// 			 i++;
// 			 // Пропускаем все символы до появления буквы (обычно завершающей escape-последовательность)
// 			 while (s[i] && !isalpha(s[i]))
// 				 i++;
// 			 if (s[i])
// 				 i++;  // пропускаем завершающий символ
// 		 }
// 		 else
// 		 {
// 			 clean[j++] = s[i++];
// 		 }
// 	 }
// 	 clean[j] = '\0';
// 	 return (clean);
//  }


 static int	ft_strcmp(const char *s1, const char *s2)
 {
	 while (*s1 && (*s1 == *s2))
	 {
		 s1++;
		 s2++;
	 }
	 return ((unsigned char)*s1 - (unsigned char)*s2);
 }

//  static char	*ft_strtrim_newline(char *s)
//  {
// 	 size_t	len;
 
// 	 if (!s)
// 		 return (NULL);
// 	 len = ft_strlen(s);
// 	 if (len > 0 && s[len - 1] == '\n')
// 		 s[len - 1] = '\0';
// 	 return (s);
//  }

 int	handle_heredoc(char *delimiter)
 {
	 int		temp_fd;
	 char	*line;
 
	 temp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	 if (temp_fd < 0)
	 {
		 perror("Error: Unable to create temporary file for here_doc");
		 return (-1);
	 }
	 while (1)
	 {
		 line = readline("heredoc> ");
		 if (!line)
			 break;
		 // Если введённая строка совпадает с delimiter, завершаем ввод.
		 if (ft_strcmp(line, delimiter) == 0)
		 {
			 free(line);
			 break;
		 }
		 // Можно добавить отладочный вывод, если необходимо:
		 write(temp_fd, line, ft_strlen(line));
		 write(temp_fd, "\n", 1);
		 free(line);
	 }
	 close(temp_fd);
	 temp_fd = open(".here_doc_tmp", O_RDONLY);
	 if (temp_fd < 0)
		 perror("Error: unable to open temporary file for here_doc");
	 unlink(".here_doc_tmp");
	 return (temp_fd);
 }
 
