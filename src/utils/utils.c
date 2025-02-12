/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:57:11 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/12 15:59:20 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
	// free_tokens(shell->tokens);
	// free_cmd(shell->cmd);
	free(shell);
}