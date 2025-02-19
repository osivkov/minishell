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


t_token *process_quotes(char **input, char quote)
{
	t_token *token;
	char	*start;
	int		len;

	len = 0;
	(*input)++;
	start = *input;
	while (**input && **input != quote)
	{
		len++;
		(*input)++;
	}
	if (**input != quote)
		return (NULL);
	(*input)++;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_substr(start, 0, len);
	token->type = T_WORD; 
	token->next = NULL;
	return (token);
}
