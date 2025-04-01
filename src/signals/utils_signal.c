/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:38:47 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 14:46:14 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

void	check_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_done = 1;
		rl_cleanup_after_signal();
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal(int mode, t_minishell *shell)
{
	(void)shell;
	if (mode == STOP_RESTORE)
	{
		signal(SIGINT, reset_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == STOP_QUIT)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, back_slash);
	}
	else if (mode == EXIT_MODE)
	{
		printf("exit\n");
		exit(0);
	}
	else if (mode == HEREDOC_PAUSE)
	{
		signal(SIGINT, child_signal_handler);
	}
}
