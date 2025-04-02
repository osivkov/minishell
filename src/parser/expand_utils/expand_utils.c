/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:24:55 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/24 13:47:29 by osivkov          ###   ########.fr       */
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

char	*hane_dol_var(t_minishell *shell, const char *str, size_t *i, char *exp)
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
	temp = ft_strjoin(exp, value);
	free(exp);
	exp = temp;
	*i = j;
	return (exp);
}

char	*h_d(t_minishell *shell, const char *str, char *exp, size_t *i)
{
	if (str[*i + 1] == '?')
		exp = handle_dollar_exit(shell, exp, i);
	else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		exp = hane_dol_var(shell, str, i, exp);
	else
	{
		exp = append_char_exp(exp, '$');
		(*i)++;
	}
	return (exp);
}
