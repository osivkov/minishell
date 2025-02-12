/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/12 16:36:12 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>


int run_minishell(t_minishell *shell)
{
	char	*input;
	(void)shell;
	// t_token	*tokens;
	// t_cmd	*cmd;

	while (1)
	{
		/* Display prompt and read input */
		input = readline("minishell> ");
		if (!input)
		{
			/* If input is NULL (Ctrl-D pressed), exit the shell */
			ft_putstr_fd("exit\n", 1);
			break;
		}

		if (input[0] != '\0')
			add_history(input); /*add_history part of readline lib, with we can use
			arrow  tow down for listing history*/

		/* Call the parser here:
		 * tokens = lexer(input);
		 * cmd = parser(tokens);
		 *
		 * For now, simply print the input as a placeholder.
		 */
		ft_putendl_fd(input, 1);  // Print the input (placeholder)

		/* Free the memory allocated by readline */
		free(input);

		/* After integrating the parser and executor, here we will:
		 * - Execute commands (execute(shell->cmd));
		 * - Free tokens and commands (free_tokens, free_cmd)
		 */
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

	(void)argc;
	(void)argv;

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