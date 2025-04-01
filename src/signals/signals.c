/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:53:37 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/28 18:45:10 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

volatile sig_atomic_t g_exit = 0;

void reset_prompt(int sig)
{
    (void)sig;
    g_exit = 130;
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void ctrl_c(int sig)
{
    (void)sig;
    g_exit = 130;
    write(STDOUT_FILENO, "\n", 1);
}

void back_slash(int sig)
{
    (void)sig;
    g_exit = 131;
    printf("Quit (core dumped)\n");
}

/* Обработчик для HEREDOC – при Ctrl+C устанавливаем g_exit и принудительно завершаем readline */
void child_signal_handler(int sig)
{
        int dev_null_fd;
        if (sig == SIGINT)
        {
        dev_null_fd = open("/dev/null", O_RDONLY);
		dup2(dev_null_fd, STDIN_FILENO);
		close(dev_null_fd);
		printf("\n");
		// rl_on_new_line();
		// rl_replace_line("", 0);
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

void	check_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}


void set_signal(int mode, t_minishell *shell)
{
    (void)shell; /* shell не нужен для этих режимов */
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
        // Можно установить другой обработчик, если нужно
        signal(SIGINT, child_signal_handler);
        signal(SIGQUIT, SIG_DFL);
    }
}



// void	handle_sigint(int sig)
// {
// 	(void)sig;

// 	// write(1, "\n", 1);
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// 	g_signal_status = sig;
// }


// void	ctrl_c(int sig)
// {
// 	g_exit = 130;
// 	/* Выводим перевод строки */
// 	write(STDOUT_FILENO, "\n", 1);
// 	(void)sig;
// }

// void	back_slash(int sig)
// {
// 	g_exit = 131;
// 	printf("Quit (core dumped)\n");
// 	(void)sig;
// }

// void	reset_prompt(int sig)
// {
// 	g_exit = 130;

// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// 	(void)sig;
	
// }

// void	set_signal(int mode, t_minishell *shell)
// {
// 	(void)shell; /* Если shell не нужен для обработки, можно проигнорировать */
// 	if (mode == STOP_RESTORE)
// 	{
// 		signal(SIGINT, reset_prompt);
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// 	else if (mode == STOP_QUIT)
// 	{
// 		signal(SIGINT, ctrl_c);
// 		signal(SIGQUIT, back_slash);
// 	}
// 	else if (mode == EXIT_MODE)
// 	{
// 		printf("exit\n");
// 		/* Здесь вызывается функция корректного завершения работы */
// 		/* clean_exit(shell, BUILTIN_EXIT); */
// 		exit(0);
// 	}
// 	else if (mode == HEREDOC)
// 	{
// 		/* Установите свой обработчик для heredoc */
// 		signal(SIGINT, child_signal_handler);
// 	}
// 	else if (mode == HEREDOC_PAUSE)
// 	{
// 		// signal(SIGINT, child_signal_handler2);
// 	}
// }


/* Обработчик SIGINT для обычного режима (reset prompt) */
// void child_signal_handler(int sig)
// {
//     (void)sig;
//     g_exit = 130;
//     write(STDOUT_FILENO, "\n", 1);
//     rl_done = 1;  /* Принудительно завершаем работу readline */
// }

// void reset_prompt(int sig)
// {
//     (void)sig;
//     g_exit = 130;
//     write(STDOUT_FILENO, "\n", 1);
//     rl_replace_line("", 0);
//     rl_on_new_line();
//     rl_redisplay();
// }

// void ctrl_c(int sig)
// {
//     (void)sig;
//     g_exit = 130;
//     write(STDOUT_FILENO, "\n", 1);
// }

// void back_slash(int sig)
// {
//     (void)sig;
//     g_exit = 131;
//     printf("Quit (core dumped)\n");
// }

// void set_signal(int mode, t_minishell *shell)
// {
//     (void)shell;
//     if (mode == STOP_RESTORE)
//     {
//         signal(SIGINT, reset_prompt);
//         signal(SIGQUIT, SIG_IGN);
//     }
//     else if (mode == STOP_QUIT)
//     {
//         signal(SIGINT, ctrl_c);
//         signal(SIGQUIT, back_slash);
//     }
//     else if (mode == EXIT_MODE)
//     {
//         printf("exit\n");
//         exit(0);
//     }
//     else if (mode == HEREDOC)
//     {
//         signal(SIGINT, child_signal_handler);
//     }
//     else if (mode == HEREDOC_PAUSE)
//     {
//         signal(SIGINT, child_signal_handler2);
//     }
// }

