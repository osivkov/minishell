/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:53:37 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 10:55:25 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

volatile sig_atomic_t			g_exit = 0;

void	reset_prompt(int sig)
{
	(void)sig;
	g_exit = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ctrl_c(int sig)
{
	(void)sig;
	g_exit = 130;
	write(STDOUT_FILENO, "\n", 1);
}

void	back_slash(int sig)
{
	(void)sig;
	g_exit = 131;
	printf("Quit (core dumped)\n");
}

void	child_signal_handler(int sig)
{
	int	dev_null_fd;

	if (sig == SIGINT)
	{
		dev_null_fd = open("/dev/null", O_RDONLY);
		dup2(dev_null_fd, STDIN_FILENO);
		close(dev_null_fd);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_exit = 130;
	}
}

void	command_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
	}
}
