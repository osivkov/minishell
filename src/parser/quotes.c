/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:49:08 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 14:42:38 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>


t_token *process_quotes(t_minishell *shell, char **input, char quote_char)
{
    t_token *token;
    char    *start;
    int     len = 0;

    // Пропускаем открывающую кавычку
    (*input)++;

    start = *input;
    // Идём, пока не найдём такую же кавычку или не дойдём до конца строки
    while (**input && **input != quote_char)
    {
        len++;
        (*input)++;
    }

    // Если дошли до конца или не встретили закрывающую кавычку
    if (**input != quote_char)
    {
        // Ошибка: нет закрывающей кавычки
        shell->last_exit = 2;
        ft_putstr_fd("minishell: syntax error: missing closing quote `", 2);
        ft_putchar_fd(quote_char, 2);
        ft_putendl_fd("'", 2);
        // fprintf(stderr, "minishell: syntax error: missing closing quote `%c`\n",
        //         quote_char);
        return NULL;
    }

    // Создаём токен
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
    {
        shell->last_exit = 2; // или 1, в зависимости от вашей логики
        fprintf(stderr, "minishell: allocation error in process_quotes\n");
        return NULL;
    }
    // Выделяем место под substring
    token->value = ft_substr(start, 0, len);
    token->type = T_WORD;
    token->next = NULL;

    // Пропускаем закрывающую кавычку
    (*input)++;

    return token;
}
