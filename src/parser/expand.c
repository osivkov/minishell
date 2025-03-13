/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:50:09 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/13 12:29:42 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// getenv - used to get the values of environment variables
/*If the user enters a command with variables 
	(for example, echo $HOME or echo $?),
	hese functions will replace them with the 
	current values ​​from the environment or the current return code.*/
// Helper function: search for the variable's value in shell->env.
// Each variable is represented as "KEY=VALUE".
char	*get_env_value(t_minishell *shell, const char *var)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var, len) == 0
			&& shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (""); // If variable not found, return an empty string.
}


char	*expand_variables_with_quotes(t_minishell *shell,
	const char *str, const int *qt_array)
{
	char	*expanded;
	size_t	i;

	expanded = ft_strdup("");
	if (!expanded)
	return (NULL);
	i = 0;
	while (str[i])
	{
	if (str[i] == '$' && qt_array[i] != 1)
		{
		expanded = handle_dollar(shell, str, qt_array, expanded, &i);
		continue ;
		}
	else
		{
		expanded = append_char_exp(expanded, str[i]);
		i++;
		}
	}
	return (expanded);
}

// Function to expand environment variables in the given string.
// It replaces occurrences of $VAR and $? with their corresponding values.
// char *expand_variables_with_quotes(t_minishell *shell, const char *str, const int *qt_array)



// Function to expand variables for each argument of every command in the command list.
void expand_command_variables(t_minishell *shell, t_cmd *cmd_list)
{
	int		i;
	char	*expanded;

	while (cmd_list)
	{
		if (cmd_list->args)
		{
			i = 0;
			while (cmd_list->args[i])
			{
				expanded = expand_variables_with_quotes(shell, cmd_list->args[i], cmd_list->quote_type[i]);
				if (expanded)
				{
					free(cmd_list->args[i]);
					cmd_list->args[i] = expanded;
				}
				// После расширения информация о кавычках для данного аргумента уже не нужна
				free(cmd_list->quote_type[i]);
				cmd_list->quote_type[i] = NULL;
				i++;
			}
		}
		cmd_list = cmd_list->next;
	}
}
