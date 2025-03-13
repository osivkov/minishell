/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:50:09 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/13 12:23:15 by osivkov          ###   ########.fr       */
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

static char	*append_char(char *expanded, char c)
{
	char	temp[2];
	char	*result;

	temp[0] = c;
	temp[1] = '\0';
	result = ft_strjoin(expanded, temp);
	free(expanded);
	return (result);
}

static char	*handle_dollar_exit(t_minishell *shell, char *expanded, size_t *i)
{
	char	*exit_str;
	char	*temp;

	exit_str = ft_itoa(shell->last_exit);
	temp = ft_strjoin(expanded, exit_str);
	free(expanded);
	expanded = temp;
	free(exit_str);
	*i += 2;
	return (expanded);
}

static char	*handle_dollar_variable(t_minishell *shell, const char *str, size_t *i, char *expanded)
{
	size_t	j;
	char	*var_name;
	char	*value;
	char	*temp;

	j = *i + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_substr(str, *i + 1, j - (*i + 1));
	value = get_env_value(shell, var_name);
	free(var_name);
	temp = ft_strjoin(expanded, value);
	free(expanded);
	expanded = temp;
	*i = j;
	return (expanded);
}

static char	*handle_dollar(t_minishell *shell, const char *str,
	const int *qt_array, char *expanded, size_t *i)
{
	(void)qt_array;
	if (str[*i + 1] == '?')
		expanded = handle_dollar_exit(shell, expanded, i);
	else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		expanded = handle_dollar_variable(shell, str, i, expanded);
	else
	{
		expanded = append_char(expanded, '$');
		(*i)++;
	}
	return (expanded);
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
		expanded = append_char(expanded, str[i]);
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
