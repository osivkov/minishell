/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/26 19:08:00 by osivkov          ###   ########.fr       */
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

// void child_signal_handler(int sig)
// {
//     (void)sig;
//     g_exit = 130;
//     write(STDOUT_FILENO, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
//  	// rl_redisplay();
// 	rl_done = 1;
// 	//  fflush(stdout);
	
//}

 int	handle_heredoc(char *delimiter)
{
	int		temp_fd;
	char	*line;
	int		backup_fd;

	/* Сохраняем текущий STDIN */
	backup_fd = dup(STDIN_FILENO);
	/* Устанавливаем специальный обработчик сигналов для heredoc */
	signal(SIGINT, child_signal_handler);
	
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
		{
			/* Восстанавливаем STDIN перед выходом */
			dup2(backup_fd, STDIN_FILENO);
			close(backup_fd);
			close(temp_fd);
			unlink(".here_doc_tmp");
			return (-1);
		}
		/* Если сигнал прерывания установлен, завершаем ввод */
		if (g_exit == 130)
		{
			free(line);
			dup2(backup_fd, STDIN_FILENO);
			close(backup_fd);
			close(temp_fd);
			unlink(".here_doc_tmp");
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
	/* Восстанавливаем STDIN после завершения heredoc */
	dup2(backup_fd, STDIN_FILENO);
	close(backup_fd);
	close(temp_fd);
	temp_fd = open(".here_doc_tmp", O_RDONLY);
	if (temp_fd < 0)
		perror("Error: unable to open temporary file for here_doc");
	unlink(".here_doc_tmp");
	return (temp_fd);
}

