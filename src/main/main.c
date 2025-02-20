/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/20 10:16:56 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// #define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE 700
#include <signal.h>

volatile	sig_atomic_t g_signal_status = 0;

int run_minishell(t_minishell *shell)
{
	char		*input;
	t_token		*tokens;
	t_token		*temp;
	t_cmd		*cmd;
	t_cmd		*cmd_temp;
	int			i;
	(void)shell;

	while (1)
	{
		/* Display prompt and read input */
		input = readline("minishell> ");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break;
		}

		if (input[0] != '\0')
			add_history(input);

		/* Lexical analysis */
		tokens = lexer(input);
		ft_putendl_fd("Tokens:", 1);
		temp = tokens;
		while (temp)
		{
			ft_putstr_fd(temp->value, 1);
			ft_putchar_fd(' ', 1);
			temp = temp->next;
		}
		ft_putchar_fd('\n', 1);

		/* Syntax analysis: group tokens into commands */
		cmd = parser(tokens);
		
		/* Expand variables for each command */
		{
			// t_cmd *tmp_cmd = cmd;
			// while (tmp_cmd)
			// {
			// 	tmp_cmd->args = expand_variables(tmp_cmd->args);
			// 	tmp_cmd = tmp_cmd->next;
			// }
		}

		/* Debug: print commands (arguments) */
		ft_putendl_fd("Commands:", 1);
		cmd_temp = cmd;
		while (cmd_temp)
		{
			i = 0;
			while (cmd_temp->args && cmd_temp->args[i])
			{
				ft_putstr_fd(cmd_temp->args[i], 1);
				ft_putchar_fd(' ', 1);
				i++;
			}
			ft_putchar_fd('\n', 1);
			cmd_temp = cmd_temp->next;
		}

		/* Free the memory allocated for this input */
		free(input);
		free_tokens(tokens);
		free_cmd(cmd);
	}
	return (0);
}



/*
 * init_minishell:
 * - Allocates memory for the t_minishell structure.
 * - Copies the environment variables.
 * - Initializes tokens and cmd fields to NULL.
 * - Sets the initial exit status (e.g., 0).
 */
t_minishell *init_minishell(char **env)
{
	t_minishell	*shell;
	int			i;
	int			env_count;

	shell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);

	/* Count the environment variables */
	env_count = 0;
	while (env[env_count])
		env_count++;

	/* Allocate memory for a copy of the environment */
	shell->env = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		 if(!shell->env[i])
		 {
			while (i > 0)
			{
				i--;
				free(shell->env[i]);
			}
			free(shell->env);
			free(shell);
			return(NULL);
		 }
		i++;
	}
	shell->env[i] = NULL;

	shell->tokens = NULL;
	shell->cmd = NULL;
	shell->last_exit = 0;
	return (shell);
}


// static void	print_tokens(t_token *tokens)
// {
// 	while(tokens)
// 	{
// 		printf("Token: '%s' (Type: %d)\n", tokens->value, tokens->type);
// 		tokens = tokens->next;
// 	}
// }

// static void	print_commands(t_cmd *cmd)
// {
// 	while (cmd)
// 	{
// 		int i = 0;
// 		printf("Commands: \n");
// 		while (cmd->args && cmd->args[i])
// 		{
// 			printf("  args[%d]: '%s'\n", i, cmd->args[i]);
// 			i++;
// 		}
// 		if (cmd->infile != 1)
// 		{
// 			printf("  infile: %d\n", cmd->infile);
// 		}
// 		if (cmd->outfile != 1)
// 			printf("  outfile: %d\n", cmd->outfile);
// 		cmd = cmd->next;
// 	}
// }

int	main(int argc, char **argv, char **env)
{
	t_minishell	*shell;
	struct sigaction sa;
	(void)argc;
	(void)argv;


	// Set up signal handler for SIGINT (Ctrl-C)
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; // Restart interrupted syscalls
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	signal(SIGQUIT, SIG_IGN);
	shell = init_minishell(env);
	if (!shell)
	{
		perror("Initialization error");
		return (1);
	}

	run_minishell(shell);
	free_minishell(shell);
	return (shell->last_exit);
}

// int main(void)
// {
// 	char *input;
// 	t_token *tokens;
// 	t_cmd *cmd;

//     while (1)
//     {
// 	// Отображаем приглашение и считываем строку
// 	input = readline("minishell_test> ");
// 	if (!input)
// 	{
// 	printf("exit\n");
// 	break; // Если введён EOF (Ctrl-D), завершаем работу
// 	}
// 	if (input[0] == '\0')
// 	{
// 	free(input);
// 	continue;
// 	}
// 	add_history(input);

// 	// Лексический анализ: разбиваем ввод на токены
// 	tokens = lexer(input);
// 	printf("\nTokens:\n");
// 	print_tokens(tokens);

// 	// Синтаксический анализ: группируем токены в команды (t_cmd)
// 	cmd = parser(tokens);
// 	printf("\nCommands:\n");
// 	print_commands(cmd);

// 	// Освобождаем память, выделенную для данной итерации
// 	free(input);
// 	free_tokens(tokens);
// 	free_cmd(cmd);
// 	printf("\n");
// 	}
// 	return 0;
// }