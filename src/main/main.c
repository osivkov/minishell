/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 10:28:25 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <limits.h>
#include <unistd.h>

void	run_noninteractive_minishell(t_minishell *shell, char **argv)
{
	int		i;
	char	**split_command;

	split_command = ft_split(argv[2], ';');
	if (split_command == NULL)
	{
		ft_malloc_error(shell);
		shell->last_exit = ENOMEM;
		return ;
	}
	i = 0;
	while (split_command[i] != NULL)
	{
		if (process_command(shell, split_command[i]))
		{
			ft_free_double(split_command, NULL, NULL, NULL);
			return ;
		}
		i++;
	}
	ft_free_double(split_command, NULL, NULL, NULL);
}

static int	run_non_interactive(char **argv, char **env)
{
	t_minishell	*shell;
	int			exit_status;

	shell = init_minishell(env);
	if (!shell)
		return (perror("Initialization error"), 1);
	run_noninteractive_minishell(shell, argv);
	exit_status = shell->last_exit;
	free_minishell(shell);
	return (exit_status);
}

static int	run_interactive(char **env)
{
	t_minishell	*shell;
	int			exit_status;

	shell = init_minishell(env);
	if (!shell)
		return (perror("Initialization error"), 1);
	set_signal(STOP_RESTORE, shell);
	clear_history();
	run_minishell(shell);
	exit_status = shell->last_exit;
	free_minishell(shell);
	return (exit_status);
}

int	main(int argc, char **argv, char **env)
{
	if (argc > 1)
	{
		if (argc != 3 || ft_strncmp(argv[1], "-c", 3) != 0)
		{
			ft_putendl_fd("Mismatch of arguments", 2);
			ft_putstr_fd("If you wish to run non-interactive minishell: ", 2);
			ft_putendl_fd("use the flag '-c' followed by commands", 2);
			ft_putstr_fd("If you wish to interactive minishell: ", 2);
			ft_putendl_fd("do not provide any arguments", 2);
			return (1);
		}
		return (run_non_interactive(argv, env));
	}
	return (run_interactive(env));
}
