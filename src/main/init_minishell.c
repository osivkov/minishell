/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:53 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 10:25:56 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <limits.h>
#include <unistd.h>

void	update_shlvlv(t_minishell *shell)
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

int	count_env(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

char	**dup_env(char **env)
{
	int		count;
	char	**dup;
	int		i;

	i = 0;
	count = count_env(env);
	dup = malloc(sizeof(char *) * (count + 1));
	if (!dup)
		return (NULL);
	while (env[i])
	{
		dup[i] = ft_strdup(env[i]);
		if (!dup[i])
		{
			while (i > 0)
			{
				i--;
				free(dup[i]);
			}
			return (free(dup), NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

t_minishell	*init_minishell(char **env)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->env = dup_env(env);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	update_shlvlv(shell);
	shell->tokens = NULL;
	shell->cmd = NULL;
	shell->last_exit = 0;
	return (shell);
}
