/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:50:09 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/11 15:56:42 by osivkov          ###   ########.fr       */
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


// static char	*expand_variables(t_minishell *shell, char *str, const int *qt_array)
// {
// 	size_t	i;
// 	char	*temp;
// 	char	*expand;
// 	char 	*exit_str;
// 	size_t	j;
// 	char	*var_name;

// 	expand = ft_strdup("");
// 	if (!expand)
// 	{
// 		return NULL;
// 		//error maloc
// 	}
// 	while (str[i])
// 	{
// 		if (str[i == '$' && qt_array != 1])
// 		{
// 			if (str[i + 1] == '?')
// 			{
// 				exit_str = ft_itoa(shell->last_exit);
// 				temp = ft_strjoin(expand, exit_str);
// 				free(expand);
// 				expand = temp;
// 				free(exit_str);
// 				i += 2;
// 			}
// 		}
// 		else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
// 		{
// 			j = i + 1;
// 			while(str[j] && ft_isalnum(str[j] || str[j == '_']))
// 			{
// 				j++;
// 			}
// 			var_name = ft_substr(str, i + 1, j - (i + 1));
			
// 		}
// 	}
	
// }

// Function to expand environment variables in the given string.
// It replaces occurrences of $VAR and $? with their corresponding values.
char *expand_variables_with_quotes(t_minishell *shell, const char *str, const int *qt_array)
{
	char	*expanded;
	char	*temp;
	size_t	i;

	expanded = ft_strdup("");	// начинаем с пустой строки
	if (!expanded)
		return (NULL);
	i = 0;
	while (str[i])
	{
		// Если встречаем '$' и он не находится в одинарных кавычках
		if (str[i] == '$' && qt_array[i] != 1)
		{
			/* Если после '$' идет '?' */
			if (str[i + 1] == '?')
			{
				char *exit_str = ft_itoa(shell->last_exit);
				temp = ft_strjoin(expanded, exit_str);
				free(expanded);
				expanded = temp;
				free(exit_str);
				i += 2;
				continue;
			}
			/* Если после '$' идет имя переменной (буква или '_') */
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				size_t j = i + 1;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				char *var_name = ft_substr(str, i + 1, j - (i + 1));
				char *value = get_env_value(shell, var_name);
				free(var_name);
				temp = ft_strjoin(expanded, value);
				free(expanded);
				expanded = temp;
				i = j;
				continue;
			}
			else
			{
				// Если после '$' не идёт ни '?' ни корректное имя переменной, просто копируем '$'
				char c[2] = {'$', '\0'};
				temp = ft_strjoin(expanded, c);
				free(expanded);
				expanded = temp;
				i++;
				continue;
			}
		}
		else
		{
			// Копируем текущий символ без изменений
			char c[2] = {str[i], '\0'};
			temp = ft_strjoin(expanded, c);
			free(expanded);
			expanded = temp;
			i++;
		}
	}
	return expanded;
}


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
