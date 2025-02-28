/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:28 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 14:43:03 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

/*
	Main lexer function.
	Breaks the input string into a linked list of tokens.
*/
#include <stdio.h>   // Для fprintf(stderr, ...)

// Предположим, что у вас есть функция free_tokens(t_token *tokens).
// Она освобождает весь список токенов, включая строки value.

t_token *lexer(t_minishell *shell, char *input)
{
    t_token *head = NULL;
    t_token *current = NULL;
    t_token *new_token = NULL;

    while (*input)
    {
        // Пропускаем пробелы
        while (*input && ft_isspace(*input))
            input++;
        // Если дошли до конца строки, выходим из цикла
        if (!*input)
            break;

        // Определяем, что за токен нужно создавать
        if (*input == '\'' || *input == '\"')
        {
            // Предположим, что process_quotes теперь принимает shell,
            // чтобы при ошибке самой выставить last_exit
            new_token = process_quotes(shell, &input, *input);
        }
        else if (*input == '|' || *input == '<' || *input == '>')
        {
            // create_special_token(&input);
            new_token = create_special_token(&input);
        }
        else
        {
            // Обычное слово
            new_token = create_word_token(&input);
        }

        // Если создание токена вернуло NULL, значит произошла ошибка 
        // (например, unmatched quotes, проблемы с malloc, и т.д.)
        if (!new_token)
        {
            // Например, process_quotes уже могла поставить shell->last_exit = 2 
            // и вывести сообщение, но если нет, мы можем сделать это здесь:
            if (shell->last_exit == 0)
            {
                shell->last_exit = 2;
                fprintf(stderr, "minishell: lexer error\n");
            }

            // Освобождаем список уже созданных токенов
            free_tokens(head);
            return NULL;
        }

        // Добавляем новый токен в конец списка
        token_to_list(&head, &current, new_token);
    }
    return head;
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
