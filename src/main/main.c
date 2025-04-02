/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:54:22 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 09:34:52 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <limits.h>
#include <unistd.h>


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

static char	*get_input(t_minishell *shell)
{
	char	*input;
	char	*prompt;

	write(STDOUT_FILENO, "\r\033[K", 4);
	prompt = generate_prompt(shell);
	input = readline(prompt);
	free(prompt);
	if (!input)
	{
		ft_putstr_fd("exit\n", 1);
		return (NULL);
	}
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

static int	process_line(t_minishell *shell, char *input)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (g_exit != 0)
	{
		shell->last_exit = g_exit;
		g_exit = 0;
	}
	tokens = lexer(shell, input);
	if (!tokens && shell->last_exit != 0)
		return (free(input), 1);
	cmd = parser(shell, tokens);
	if (!cmd && shell->last_exit != 0)
		return (free(input), free_tokens(tokens), 1);
	expand_command_variables(shell, cmd);
	shell->cmd = cmd;
	set_signal(STOP_RESTORE, shell);
	execute(shell);
	free_tokens(tokens);
	shell->tokens = NULL;
	free_cmd(cmd);
	shell->cmd = NULL;
	free(input);
	return (0);
}

int	run_minishell(t_minishell *shell)
{
	char	*input;

	while (1)
	{
		input = get_input(shell);
		if (!input)
			break;
		if (process_line(shell, input))
			continue;
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
	env_count = 0;
	while (env[env_count])
		env_count++;
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
	int	exit_status;

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
		shell = init_minishell(env);
		if (shell == NULL)
			return (perror("Initialization error"), 1);
		run_noninteractive_minishell(shell, argv);
		exit_status = shell->last_exit;
		free_minishell(shell);
		return (exit_status);
	}
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

