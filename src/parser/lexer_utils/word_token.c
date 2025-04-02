/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:42:44 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 14:28:06 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

int	handle_dollar_quote(t_minishell *shell,
	char **input, char **value, int **qt)
{
	(*input)++;
	if (append_char(value, qt, 0x01, 0) < 0)
	{
		free(*value);
		free(*qt);
		ft_putendl_fd("minishell: malloc error", 2);
		shell->last_exit = 1;
		return (-1);
	}
	return (0);
}

int	handle_single_quote(t_minishell *shell,
	char **input, char **value, int **qt)
{
	int	ret;

	(*input)++;
	while (**input && **input != '\'')
	{
		ret = append_char(value, qt, **input, 1);
		if (ret < 0)
		{
			ft_putendl_fd("malloc error", 2);
			shell->last_exit = 1;
			return (-1);
		}
		(*input)++;
	}
	if (**input != '\'')
	{
		ft_putendl_fd("minishell: syntax error: "
			"missing closing single quote", 2);
		shell->last_exit = 2;
		return (-1);
	}
	(*input)++;
	return (0);
}

int	handle_double_quote(t_minishell *shell,
	char **input, char **value, int **qt)
{
	int	ret;

	(*input)++;
	while (**input && **input != '\"')
	{
		ret = append_char(value, qt, **input, 2);
		if (ret < 0)
		{
			ft_putendl_fd("malloc error", 2);
			shell->last_exit = 1;
			return (-1);
		}
		(*input)++;
	}
	if (**input != '\"')
	{
		ft_putendl_fd("minishell: syntax error: missing "
			"closing double quote", 2);
		shell->last_exit = 2;
		return (-1);
	}
	(*input)++;
	return (0);
}

int	handle_unquoted_char(t_minishell *shell,
	char **input, char **value, int **qt)
{
	int	ret;

	ret = append_char(value, qt, **input, 0);
	if (ret < 0)
	{
		free(*value);
		free(*qt);
		ft_putendl_fd("malloc error", 2);
		shell->last_exit = 1;
		return (-1);
	}
	(*input)++;
	return (0);
}

t_token	*create_word_token(t_minishell *shell, char **input)
{
	t_token	*token;
	char	*value;
	int		*quote_type;

	value = build_word_value(shell, input, &quote_type);
	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		ft_putendl_fd("malloc error", 2);
		free(value);
		free(quote_type);
		shell->last_exit = 1;
		return (NULL);
	}
	token->value = value;
	token->type = T_WORD;
	token->qt_array = quote_type;
	token->next = NULL;
	return (token);
}
