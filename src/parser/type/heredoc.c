/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 17:41:28 by osivkov          ###   ########.fr       */
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

static void	cleanup_heredoc_error(t_heredoc_context *ctx)
{
	dup2(ctx->backup_fd, STDIN_FILENO);
	close(ctx->backup_fd);
	close(ctx->temp_fd);
	unlink(".here_doc_tmp");
	signal(SIGINT, ctx->old_handler);
}

static int	process_heredoc_lines(t_heredoc_context *ctx, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			return (cleanup_heredoc_error(ctx), 0);
		if (g_exit == 130)
		{
			free(line);
			cleanup_heredoc_error(ctx);
			g_exit = 0;
			return (0);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(ctx->temp_fd, line, ft_strlen(line));
		write(ctx->temp_fd, "\n", 1);
		free(line);
	}
	return (1);
}

static int	finalize_heredoc(t_heredoc_context *ctx)
{
	dup2(ctx->backup_fd, STDIN_FILENO);
	close(ctx->backup_fd);
	close(ctx->temp_fd);
	ctx->temp_fd = open(".here_doc_tmp", O_RDONLY);
	if (ctx->temp_fd < 0)
		perror("Error: unable to open temporary file for here_doc");
	unlink(".here_doc_tmp");
	g_exit = 0;
	signal(SIGQUIT, SIG_DFL);
	return (ctx->temp_fd);
}

int	handle_heredoc(char *delimiter)
{
	t_heredoc_context	ctx;

	ctx.backup_fd = dup(STDIN_FILENO);
	ctx.old_handler = signal(SIGINT, child_signal_handler);
	ctx.temp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ctx.temp_fd < 0)
	{
		perror("Error: Unable to create temporary file for here_doc");
		signal(SIGINT, ctx.old_handler);
		return (-1);
	}
	if (!process_heredoc_lines(&ctx, delimiter))
		return (-1);
	return (finalize_heredoc(&ctx));
}
