/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_minishell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:19:21 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 10:28:08 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <limits.h>
#include <unistd.h>

int	process_command(t_minishell *shell, char *command)
{
	t_token	*tokens;
	t_cmd	*cmd;

	tokens = lexer(shell, command);
	if (!tokens && shell->last_exit == 2)
		return (1);
	cmd = parser(shell, tokens);
	if (!cmd && shell->last_exit == 2)
		return (free_tokens(tokens), 1);
	expand_command_variables(shell, cmd);
	shell->cmd = cmd;
	if (shell->cmd == NULL)
	{
		shell->last_exit = 2;
		printf("shell-cmd is NULL\n");
		free_tokens(tokens);
		return (1);
	}
	execute(shell);
	free_tokens(tokens);
	shell->tokens = NULL;
	free_cmd(cmd);
	shell->cmd = NULL;
	return (0);
}

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
	if (!temp)
	{
		ft_printf("Generate_promp1");
		return (NULL);
	}
	prompt = ft_strjoin(temp, cwd);
	if (!prompt)
	{
		ft_printf("Generate_promp2");
		return (NULL);
	}
	free(temp);
	temp = ft_strjoin(prompt, "$ ");
	free(prompt);
	prompt = temp;
	return (prompt);
}

char	*get_input(t_minishell *shell)
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

int	process_line(t_minishell *shell, char *input)
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
			break ;
		if (process_line(shell, input))
			continue ;
	}
	return (0);
}
