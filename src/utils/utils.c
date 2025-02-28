/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:57:11 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 14:44:26 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;
	int i;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		// Освобождаем массив аргументов
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i])
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		free(tmp);
	}
}


void free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}


/*
* free_minishell:
* - Frees the memory allocated for environment variables.
* - Frees the token and command lists.
* - Frees the shell structure itself.
*/

void free_minishell(t_minishell *shell)
{
	int i;

	if (!shell)
		return;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
		i++;
		}
		free(shell->env);
	}
	// for the tokens and cmd struct;
	free_tokens(shell->tokens);
	free_cmd(shell->cmd);
	free(shell);
}


int	ft_isspace(int c)
{
	if (c == ' ' ||
		c == '\t' ||
		c == '\n' ||
		c == '\v' ||
		c == '\f' ||
		c == '\f' ||
		c == '\r')
	{
		return (1);
	}	
	return (0);
}