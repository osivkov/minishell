/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:28 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 18:06:15 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
#include <stdio.h>

static t_token	*get_next_token(t_minishell *shell, char **input)
{
	t_token	*new_token;

	while (**input && ft_isspace(**input))
		(*input)++;
	if (!**input)
		return (NULL);
	if (**input == '|' || **input == '<' || **input == '>')
		new_token = create_special_token(input);
	else
		new_token = create_word_token(shell, input);
	return (new_token);
}

t_token	*lexer(t_minishell *shell, char *inputs)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	head = NULL;
	current = NULL;
	while (*inputs)
	{
		new_token = get_next_token(shell, &inputs);
		if (!new_token)
		{
			if (!*inputs)
				break ;
			if (shell->last_exit == 0)
			{
				shell->last_exit = 2;
				ft_putendl_fd("minishell: syntax"
					"error near unexpected token", 2);
			}
			free_tokens(head);
			return (NULL);
		}
		token_to_list(&head, &current, new_token);
	}
	return (head);
}
