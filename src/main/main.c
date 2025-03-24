/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/24 14:29:15 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// #define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE 700
#include <signal.h>
#include <bits/sigaction.h>
#include <limits.h>  // for PATH_MAX
#include <unistd.h>  // for getcwd


volatile	sig_atomic_t g_signal_status = 0;


char	*generate_prompt(t_minishell *shell)
{
	char	*user;
	char	cwd[PATH_MAX];
	char	*temp;
	char	*prompt;

	user = get_env_value(shell, "USER");
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		cwd[0] = '\0';
	temp = ft_strjoin(user, "@");
		if(!temp)
		{
			ft_printf("Generate_promp1");
			return (NULL);
		}
	prompt = ft_strjoin(temp, cwd);
	if (!prompt)
	{
		ft_printf("Generate_promp2");
		return(NULL);
	}
	free(temp);
	temp = ft_strjoin(prompt, "$ ");
	free(prompt);
	prompt = temp;
	return (prompt);
}

int	run_minishell(t_minishell *shell)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmd;
	char	*prompt;

	while (1)
	{
		// Generate the prompt dynamically (e.g., "user@cwd$ ")
		prompt = generate_prompt(shell);
		// Read input from the user using the generated prompt
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			// If readline returns NULL, it indicates EOF (e.g., Ctrl+D)
			ft_putstr_fd("exit\n", 1);
			break;
		}
		// If the input is not empty, add it to the history
		if (input[0] != '\0')
			add_history(input);
		// LEXER: Convert the input string into a list of tokens
		tokens = lexer(shell, input);
		// debug_print_tokens(tokens);
		
		// If a lexer error occurs (e.g., unmatched quotes),
		// shell->last_exit is set to 2 and tokens is NULL
		if (!tokens && shell->last_exit == 2)
		{
			free(input);
			continue; // Skip parser/execution and prompt for new input
		}
		// PARSER: Build a command list (t_cmd) from the token list
		cmd = parser(shell, tokens);
		// print_cmds(cmd);
		// If a parser error occurs, free tokens and input, then prompt again
		if (!cmd && shell->last_exit == 2)
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		// Expand environment variables in all command arguments
		expand_command_variables(shell, cmd);
		// Here, you can call your (currently simplified) execute function
		shell->cmd = cmd;
		if (shell->cmd == NULL)
			printf("shell-cmd is NULL\n");
		// pseudo_execute(shell);
		execute(shell); 
		// Free tokens, command list, and input after execution
		free_tokens(tokens);
		tokens = NULL;
		shell->tokens = NULL;
		free_cmd(cmd);
		cmd = NULL;
		shell->cmd = NULL;
		free(input);
		// free(prompt);
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

static void update_shlvlv(t_minishell *shell)
{
	char	*shlvlv_str;
	int		shlvl;
	char	*new_value;

	shlvlv_str = get_env_value(shell, "SHLVL");
	if (!shlvlv_str)
	{
		ft_set_env_var(shell, "SHLVL", "1");
		return ;
	}
	shlvl = atoi(shlvlv_str);
	shlvl++;
	if (shlvl > 1000)
	{
		shlvl = 1;
	}
	new_value = ft_itoa(shlvl);
	ft_set_env_var(shell, "SHLVL", new_value);
	free(new_value);
}

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
	update_shlvlv(shell);
	shell->env[i] = NULL;
	shell->tokens = NULL;
	shell->cmd = NULL;
	shell->last_exit = 0;
	return (shell);
}

void	run_noninteractive_minishell(t_minishell *shell, char **argv)
{
	int		i;
	t_token	*tokens;
	t_cmd	*cmd;
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
		tokens = lexer(shell, split_command[i]);
		if (!tokens && shell->last_exit == 2)
		{
			ft_free_double(split_command, NULL, NULL, NULL);
			return ;
		}
		cmd = parser(shell, tokens);
		if (!cmd && shell->last_exit == 2)
		{
			free_tokens(tokens);
			ft_free_double(split_command, NULL, NULL, NULL);
			return ;
		}
		expand_command_variables(shell, cmd);
		shell->cmd = cmd;
		if (shell->cmd == NULL)
		{
			shell->last_exit = 2;
			printf("shell-cmd is NULL\n");
			free_tokens(tokens);
			return ;
		}
		execute(shell);
		free_tokens(tokens);
		shell->tokens = NULL;
		free_cmd(cmd);
		shell->cmd = NULL;
		i++;
	}
	ft_free_double(split_command, NULL, NULL, NULL);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*shell;
	struct sigaction sa;
	int	exit_status;

	if (argc > 1)
	{
		if (argc != 3 || ft_strncmp(argv[1], "-c", 3) != 0)
		{
			ft_putendl_fd("Mismatch of arguments", 2);
			ft_putstr_fd("If you wish to run non-interactive minishell: ", 2);
			ft_putendl_fd("use the flag \'-c\' followed by commands", 2);
			ft_putstr_fd("If you wish to interactive minishell: ", 2);
			ft_putendl_fd("do not provide any arguments", 2);			
			return (1);
		}
		shell = init_minishell(env);
		if (shell == NULL)
			return (perror("Initiatlization error"), 1);
		run_noninteractive_minishell(shell, argv);
		exit_status = shell->last_exit;
		free_minishell(shell);
		return (exit_status);
	}

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
	exit_status = shell->last_exit;
	free_minishell(shell);
	return (exit_status);
}
