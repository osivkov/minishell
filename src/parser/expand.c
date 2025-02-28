/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:50:09 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 15:21:16 by osivkov          ###   ########.fr       */
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
static char	*get_env_value(t_minishell *shell, const char *var)
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

// Function to expand environment variables in the given string.
// It replaces occurrences of $VAR and $? with their corresponding values.
static char	*expand_variables(t_minishell *shell, char *str)
{
	size_t	i;
	char	*result;
	char	*temp;
	char	*exit_str;
	char	temp_char[2];
	char	*var_name;
	char	*value;
	size_t	j;

	i = 0;
	result = ft_strdup(""); // Start with an empty string.
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				exit_str = ft_itoa(shell->last_exit);
				temp = ft_strjoin(result, exit_str);
				free(result);
				result = temp;
				free(exit_str);
				i += 2;
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				j = i + 1;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				var_name = ft_substr(str, i + 1, j - (i + 1));
				value = get_env_value(shell, var_name);
				free(var_name);
				temp = ft_strjoin(result, value);
				free(result);
				result = temp;
				i = j;
			}
			else
			{
				temp_char[0] = '$';
				temp_char[1] = '\0';
				temp = ft_strjoin(result, temp_char);
				free(result);
				result = temp;
				i++;
			}
		}
		else
		{
			temp_char[0] = str[i];
			temp_char[1] = '\0';
			temp = ft_strjoin(result, temp_char);
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}

// Function to expand variables for each argument of every command in the command list.
void	expand_command_variables(t_minishell *shell, t_cmd *cmd_list)
{
	t_cmd	*cmd;
	int		i;
	char	*expanded;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				expanded = expand_variables(shell, cmd->args[i]);
				if (expanded)
				{
					free(cmd->args[i]);
					cmd->args[i] = expanded;
				}
				i++;
			}
		}
		cmd = cmd->next;
	}
}
