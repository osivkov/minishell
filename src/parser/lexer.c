/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:28 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/20 10:58:00 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
/*
	Main lexer function.
	Breaks the input string into a linked list of tokens.
*/
t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	head = NULL;
	current = NULL;
	while (*input)
	{
		while (*input && ft_isspace(*input))
			input++;
		if (!*input)
			break ;
		if (*input == '\'' || *input == '\"')
			new_token = process_quotes(&input, *input);
		else if (*input == '|' || *input == '<' || *input == '>')
			new_token = create_special_token(&input);
		else
			new_token = create_word_token(&input);
		if (!new_token)
			return (NULL);
		token_to_list(&head, &current, new_token);
	}
	return (head);
}

// t_token *lexer(char *input)
// {
// 	t_token *head = NULL;
// 	t_token *current = NULL;
// 	while (*input)
// 	{
// 		// Skip spaces
// 		while (*input && ft_isspace(*input))
// 			input++;
// 		if (!*input)
// 			break;
// 		t_token *new_token = malloc(sizeof(t_token));
// 		if (!new_token)
// 			return (NULL);
// 		 if (*input == '\'' || *input == '\"')
// 		{
// 			free(new_token);
// 			t_token *quoted_token = process_quotes(&input, *input);
// 			if (!quoted_token)
// 			// Handle error for unclosed quote
// 			return (NULL);
// 			new_token = quoted_token;
// 			// Add quoted_token to your linked list of tokens
// 		}
// 		// If a special character is encountered
// 		else if (*input == '|' || *input == '<' || *input == '>')
// 		{
// 			if (*input == '<' && *(input + 1) == '<')
// 			{
// 				new_token->value = ft_strdup("<<");
// 				new_token->type = T_HEREDOC;
// 				input += 2;
// 			}
// 			else if (*input == '>' && *(input + 1) == '>')
// 			{
// 				new_token->value = ft_strdup(">>");
// 				new_token->type = T_REDIR_APPEND;
// 				input += 2;
// 			}
// 			else
// 			{
// 			new_token->value = ft_strdup((char[]){*input, '\0'});
// 			if (*input == '|')
// 				new_token->type = T_PIPE;
// 			else if (*input == '<')
// 				new_token->type = T_REDIR_IN;
				
// 			else if (*input == '>')
// 				new_token->type = T_REDIR_OUT;
// 			input++; // Move to the next character
// 			}
// 		}
// 		else
// 		{
// 			// Handle a normal word
// // Gather characters until a space or special character is encountered
// 			char *start = input;
//while (*input && !ft_isspace(*input) && *input != '|' && *input != '<' && *input != '>')
// 				input++;
// 			int len = input - start;
// 			new_token->value = ft_substr(start, 0, len);
// 			new_token->type = T_WORD;
// 		}
// 		new_token->next = NULL;
// 		// Add the new token to the linked list
// 		if (!head)
// 		{
// 			head = new_token;
// 			current = new_token;
// 		}
// 		else
// 		{
// 			current->next = new_token;
// 			current = new_token;
// 		}
// 	}
// 	return (head);
// }
