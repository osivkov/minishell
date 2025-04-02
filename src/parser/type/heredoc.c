/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:01:49 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 15:26:07 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

void	heredoc_child(t_heredoc_context *ctx, char *delimiter)
{
	if (!process_heredoc_lines(ctx, delimiter))
		exit(1);
	finalize_heredoc(ctx);
	exit(0);
}

int	heredoc_parent(t_heredoc_context *ctx, pid_t pid)
{
	int		status;
	int		fd;

	signal(SIGINT, ctx->old_handler);
	waitpid(pid, &status, 0);
	dup2(ctx->backup_fd, STDIN_FILENO);
	close(ctx->backup_fd);
	close(ctx->temp_fd);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		unlink(".here_doc_tmp");
		return (-1);
	}
	fd = open(".here_doc_tmp", O_RDONLY);
	if (fd < 0)
	{
		unlink(".here_doc_tmp");
		return (-1);
	}
	unlink(".here_doc_tmp");
	return (fd);
}

int	handle_heredoc(char *delimiter)
{
	pid_t				pid;
	t_heredoc_context	ctx;

	ctx.backup_fd = dup(STDIN_FILENO);
	ctx.old_handler = signal(SIGINT, child_signal_handler);
	ctx.temp_fd = open(".here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ctx.temp_fd < 0)
	{
		perror("Error: Unable to create temporary file for heredoc");
		signal(SIGINT, ctx.old_handler);
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		return (-1);
	}
	else if (pid == 0)
		heredoc_child(&ctx, delimiter);
	else
		return (heredoc_parent(&ctx, pid));
	return (-1);
}
