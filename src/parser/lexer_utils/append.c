/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:52:54 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 17:27:57 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char	*realloc_and_append_char(char *old_str, size_t old_len, char c)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * (old_len + 2));
	if (!new_str)
		return (NULL);
	if (old_str)
	{
		ft_memcpy(new_str, old_str, old_len);
		free(old_str);
	}
	new_str[old_len] = c;
	new_str[old_len + 1] = '\0';
	return (new_str);
}

static int	*realloc_and_append_int(int *old_qt, size_t old_len, int qtype)
{
	int	*new_qt;

	new_qt = malloc(sizeof(int) * (old_len + 1));
	if (!new_qt)
		return (NULL);
	if (old_qt)
	{
		ft_memcpy(new_qt, old_qt, sizeof(int) * old_len);
		free(old_qt);
	}
	new_qt[old_len] = qtype;
	return (new_qt);
}

int	append_char(char **str, int **qt, char c, int qtype)
{
	size_t	old_len;
	char	*new_str;
	int		*new_qt;

	if (!*str)
		old_len = 0;
	else
		old_len = ft_strlen(*str);
	new_str = realloc_and_append_char(*str, old_len, c);
	if (!new_str)
		return (-1);
	new_qt = realloc_and_append_int(*qt, old_len, qtype);
	if (!new_qt)
	{
		free(new_str);
		return (-1);
	}
	*str = new_str;
	*qt = new_qt;
	return (0);
}

int	process_chars(t_minishell *shell, char **input,
	char **value, int **quote_type)
{
	while (**input && !ft_isspace(**input) && !is_operator_char(**input))
	{
		if (**input == '\'')
		{
			if (handle_single_quote(shell, input, value, quote_type) < 0)
				return (-1);
		}
		else if (**input == '\"')
		{
			if (handle_double_quote(shell, input, value, quote_type) < 0)
				return (-1);
		}
		else
		{
			if (handle_unquoted_char(shell, input, value, quote_type) < 0)
				return (-1);
		}
	}
	return (0);
}

char	*build_word_value(t_minishell *shell, char **input, int **out_qt)
{
	char	*value;
	int		*quote_type;

	value = NULL;
	quote_type = NULL;
	if (**input == '$' && ((*input)[1] == '\'' || (*input)[1] == '\"'))
	{
		if (handle_dollar_quote(shell, input, &value, &quote_type) < 0)
		{
			free(quote_type);
			shell->last_exit = ENOMEM;
			return (free(value), NULL);
		}
	}
	if (process_chars(shell, input, &value, &quote_type) < 0)
	{
		free(value);
		free(quote_type);
		shell->last_exit = ENOMEM;
		return (NULL);
	}
	if (!value)
		return (free(quote_type), NULL);
	*out_qt = quote_type;
	return (value);
}
