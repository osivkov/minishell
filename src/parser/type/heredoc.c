/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/28 18:52:27 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

static int	ft_strcmp(const char *s1, const char *s2)
 {
	 while (*s1 && (*s1 == *s2))
	 {
		 s1++;
		 s2++;
	 }
	 return ((unsigned char)*s1 - (unsigned char)*s2);
 }


int	handle_heredoc(char *delimiter)
{
	int		temp_fd;
	char	*line;
	int		backup_fd;
	/* Сохраняем текущий STDIN */
	backup_fd = dup(STDIN_FILENO);
	/* Сохраняем старый обработчик SIGINT и устанавливаем child_signal_handler */
	void (*old_handler)(int) = signal(SIGINT, child_signal_handler);
	temp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		perror("Error: Unable to create temporary file for here_doc");
		/* Восстанавливаем старый обработчик перед выходом */
		signal(SIGINT, old_handler);
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			dup2(backup_fd, STDIN_FILENO);
			close(backup_fd);
			close(temp_fd);
			unlink(".here_doc_tmp");
			/* Восстанавливаем старый обработчик SIGINT */
			signal(SIGINT, old_handler);
			return (-1);
		}
		/* Если был получен сигнал Ctrl+C, прерываем ввод */
		if (g_exit == 130)
		{
			free(line);
			dup2(backup_fd, STDIN_FILENO);
			close(backup_fd);
			close(temp_fd);
			unlink(".here_doc_tmp");
			/* Сбрасываем g_exit, так как сигнал уже обработан */
			g_exit = 0;
			/* Восстанавливаем старый обработчик SIGINT */
			signal(SIGINT, old_handler);
			return (-1);
		}
		/* Если введённая строка совпадает с delimiter, завершаем ввод */
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		/* Записываем строку во временный файл */
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	dup2(backup_fd, STDIN_FILENO);
	close(backup_fd);
	close(temp_fd);
	temp_fd = open(".here_doc_tmp", O_RDONLY);
	if (temp_fd < 0)
		perror("Error: unable to open temporary file for here_doc");
	unlink(".here_doc_tmp");
	/* Сбрасываем g_exit после успешного завершения heredoc */
	g_exit = 0;
	/* Восстанавливаем старый обработчик SIGINT */
	signal(SIGQUIT, SIG_DFL);
	return (temp_fd);
}


