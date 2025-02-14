/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:49:08 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/14 14:42:22 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include <stdlib.h>

// Helper function to process quoted strings in the lexer.
// 'quote' is the opening quote character (' or ").
// The function returns a newly allocated token containing the content inside the quotes.
t_token *process_quotes(char **input, char quote)
{
	t_token *token;
	char	*start;
	int		len = 0;

	// Move past the opening quote.
	(*input)++;
	start = *input;
	// Find the closing quote.
	while (**input && **input != quote)
	{
		len++;
		(*input)++;
	}
	// If the closing quote was not found, handle error (for now, we return NULL).
	if (**input != quote)
		return (NULL);
	// Move past the closing quote.

	(*input)++;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_substr(start, 0, len);
	token->type = T_WORD; 
	token->next = NULL;
	return (token);
}
