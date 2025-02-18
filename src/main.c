/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/18 14:57:58 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>


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
			t_cmd *tmp_cmd = cmd;
			while (tmp_cmd)
			{
				tmp_cmd->args = expand_variables(tmp_cmd->args);
				tmp_cmd = tmp_cmd->next;
			}
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