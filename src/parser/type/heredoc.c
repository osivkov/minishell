/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/19 15:35:07 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
/*
 * handle_heredoc:
 *  - Takes a delimiter string.
 *  - Creates a temporary file.
 *  - Reads lines from the user (using readline) until the input exactly matches the delimiter.
 *  - Writes each line (with a newline) into the temporary file.
 *  - Resets the file offset to the beginning.
 *  - Returns the file descriptor for the temporary file.
 */

int	handle_heredoc(char *delimeter)
{
	char	*line;
	int		temp_fd;

	temp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
		perror("Error: Unable to create temporary file for here_doc");
	while (1)
	{
		write(1, "heredoc> ",9);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, delimeter, ft_strlen(delimeter)) == 0)
		{
			free(line);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n",1);
		free(line);
	}
	close(temp_fd);
	temp_fd = open(".here_doc_tmp", O_RDONLY);
	if (temp_fd < 0)
		perror("Error: unable to open temporary file for here_doc");
	return (temp_fd);
}
