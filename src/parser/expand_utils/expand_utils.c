/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:24:55 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/13 12:30:37 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


char	*append_char_exp(char *expanded, char c)
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

char	*handle_dollar(t_minishell *shell, const char *str,const int *qt_array, char *expanded, size_t *i)
{
	(void)qt_array;
	if (str[*i + 1] == '?')
		expanded = handle_dollar_exit(shell, expanded, i);
	else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		expanded = handle_dollar_variable(shell, str, i, expanded);
	else
	{
		expanded = append_char_exp(expanded, '$');
		(*i)++;
	}
	return (expanded);
}
