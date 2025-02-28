/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 15:13:40 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// #define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE 700
#include <signal.h>
#include <bits/sigaction.h>


volatile	sig_atomic_t g_signal_status = 0;

int run_minishell(t_minishell *shell)
{
    char        *input;
    t_token     *tokens;
    t_cmd       *cmd;

    while (1)
    {
        // Выводим приглашение и считываем ввод
        input = readline("minishell> ");
        if (!input)
        {
            // readline вернул NULL => EOF (Ctrl+D)
            ft_putstr_fd("exit\n", 1);
            break;
        }

        // Если строка не пустая, добавляем в историю
        if (input[0] != '\0')
            add_history(input);

        // ЛЕКСЕР: преобразуем строку в список токенов
        tokens = lexer(shell, input);
        // Если произошла ошибка лексера, например незакрытые кавычки,
        // shell->last_exit может быть = 2, а tokens = NULL
        if (!tokens && shell->last_exit == 2)
        {
            free(input);
            continue; // пропускаем парсер/исполнитель, предлагаем новый ввод
        }

        // ПАРСЕР: строим список команд (t_cmd) из списка токенов
        cmd = parser(shell, tokens);
        // Аналогичная проверка на ошибку парсера
        if (!cmd && shell->last_exit == 2)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }
		expand_command_variables(shell, cmd);
        // Здесь можно вызвать вашу (пока упрощённую) функцию execute
        // execute(shell, cmd);

        // Освобождаем токены, команды и саму строку
        free_tokens(tokens);
        free_cmd(cmd);
        free(input);
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



int	main(int argc, char **argv, char **env)
{
	t_minishell	*shell;
	struct sigaction sa;
	(void)argc;
	(void)argv;


	// Set up signal handler for SIGINT (Ctrl-C)
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // Restart interrupted syscalls
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