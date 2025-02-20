/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 10:50:11 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/20 11:07:29 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
/*
	Static function to add a token to the list.
*/
void	token_to_list(t_token **head, t_token **current, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*current = new_token;
	}
	else
	{
		(*current)->next = new_token;
		*current = new_token;
	}
}

/*
	Static function to create a token for double operators (<< or >>).
*/
t_token	*create_double_operator_token(char **input)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	if (**input == '<' && *(*input + 1) == '<')
	{
		token->value = ft_strdup("<<");
		token->type = T_HEREDOC;
	}
	else if (**input == '>' && *(*input + 1) == '>')
	{
		token->value = ft_strdup(">>");
		token->type = T_REDIR_APPEND;
	}
	*input += 2;
	return (token);
}
/*
	Static function to create a token for a single operator (|, <, >).
*/

t_token	*create_single_operator_token(char **input)
{
	t_token	*token;
	char	tmp[2];

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	{
		tmp[0] = **input;
		tmp[1] = '\0';
		token->value = ft_strdup(tmp);
	}
	if (**input == '|')
		token->type = T_PIPE;
	else if (**input == '<')
		token->type = T_REDIR_IN;
	else if (**input == '>')
		token->type = T_REDIR_OUT;
	(*input)++;
	return (token);
}

t_token	*create_special_token(char **input)
{
	if ((**input == '<' && *(*input + 1) == '<')
		|| (**input == '>' && *(*input + 1) == '>'))
		return (create_double_operator_token(input));
	else
		return (create_single_operator_token(input));
}

/*
	Static function to create a word token.
	Gathers characters until a space or operator is encountered.
*/
t_token	*create_word_token(char **input)
{
	char	*start;
	int		len;
	t_token	*token;

	token = malloc(sizeof(t_token));
	len = 0;
	start = *input;
	if (!token)
		return (NULL);
	token->next = NULL;
	while (**input && !ft_isspace(**input) && **input != '|'
		&& **input != '<' && **input != '>')
	{
		len++;
		(*input)++;
	}
	token->value = ft_substr(start, 0, len);
	token->type = T_WORD;
	return (token);
}
