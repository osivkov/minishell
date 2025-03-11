/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 10:50:11 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/10 18:08:31 by osivkov          ###   ########.fr       */
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



#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * is_operator_char: проверяет, не является ли текущий символ 
 *                  началом оператора (|, <, >).
 * Вы можете подправить под свою реализацию (проверять <<, >> и т.п.)
 */


static int append_char(char **str, int **qt, char c, int qtype)
{
    size_t  old_len;
    char    *new_str;
    int     *new_qt;

    if (!*str)
        old_len = 0;
    else
        old_len = ft_strlen(*str);

    // Переаллоцируем место под старую строку + 1 символ + '\0'
    new_str = (char *)malloc(sizeof(char) * (old_len + 2));
    if (!new_str)
        return (-1);

    // Переаллоцируем массив типов кавычек (на 1 элемент больше)
    new_qt = (int *)malloc(sizeof(int) * (old_len + 1));
    if (!new_qt)
    {
        free(new_str);
        return (-1);
    }

    // Скопируем старое содержимое (если было)
    if (*str)
    {
        ft_memcpy(new_str, *str, old_len);
        ft_memcpy(new_qt, *qt, sizeof(int) * old_len);
        free(*str);
        free(*qt);
    }

    // Добавляем новый символ и завершающий нуль
    new_str[old_len] = c;
    new_str[old_len + 1] = '\0';

    // Добавляем соответствующий тип кавычки
    new_qt[old_len] = qtype;

    // Обновляем указатели
    *str = new_str;
    *qt  = new_qt;
    return (0);
}


static int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/*
 * create_word_token:
 *   Считывает одно "слово" (токен) из *input, учитывая кавычки.
 *   Возвращает указатель на t_token (T_WORD). При ошибке (например, 
 *   незакрытые кавычки) возвращает NULL и выставляет shell->last_exit.
 */
t_token *create_word_token(t_minishell *shell, char **input)
{
    t_token *token;
    char    *value;         // временный буфер для символов
    int     *quote_type;    // параллельный массив: 0=unquoted, 1=single, 2=double
    int     ret;

    value = NULL;
    quote_type = NULL;

    // Пока не встретим пробел, оператор, конец строки
    while (**input && !ft_isspace(**input) && !is_operator_char(**input))
    {
        // Если одиночная кавычка
        if (**input == '\'')
        {
            (*input)++; // пропускаем саму кавычку '
            while (**input && **input != '\'')
            {
                ret = append_char(&value, &quote_type, **input, 1 /* single */);
                if (ret < 0)
                    return (ft_putendl_fd("malloc error", 2), shell->last_exit=12, NULL);
                (*input)++;
            }
            if (**input != '\'')
            {
                // не нашли закрывающую кавычку
                ft_putendl_fd("minishell: syntax error: missing closing single quote", 2);
                shell->last_exit = 2;
                free(value);
                free(quote_type);
                return NULL;
            }
            // пропускаем закрывающую кавычку
            (*input)++;
        }
        // Если двойная кавычка
        else if (**input == '\"')
        {
            (*input)++; // пропускаем "
            while (**input && **input != '\"')
            {
                ret = append_char(&value, &quote_type, **input, 2 /* double */);
                if (ret < 0)
                    return (ft_putendl_fd("malloc error", 2), shell->last_exit=12, NULL);
                (*input)++;
            }
            if (**input != '\"')
            {
                ft_putendl_fd("minishell: syntax error: missing closing double quote", 2);
                shell->last_exit = 2;
                free(value);
                free(quote_type);
                return NULL;
            }
            // пропускаем закрывающую кавычку
            (*input)++;
        }
        else
        {
            // Обычный символ (unquoted)
            ret = append_char(&value, &quote_type, **input, 0 /* unquoted */);
            if (ret < 0)
                return (ft_putendl_fd("malloc error", 2), shell->last_exit=12, NULL);
            (*input)++;
        }
    }

    // Если мы вообще не записали ни одного символа — значит пусто (не обяз. ошибка)
    if (!value)
        return NULL;

    // Создаём сам t_token
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
    {
        ft_putendl_fd("malloc error", 2);
        free(value);
        free(quote_type);
        shell->last_exit = 12;
        return NULL;
    }
    token->value      = value;
    token->type       = T_WORD; // или просто = 0, если у вас enum
         	// <-- Обычно у вас quote_type – int, но 
	token->qt_array = quote_type;//    здесь вы храните массив (придётся его где-то сохранить).
                                //    Можете заменить на собственное поле.
    token->next       = NULL;

    /*
     * Если вы хотите сохранить массив int (quote_type) в самом token-е,
     * то измените структуру t_token: сделайте, например,
     *   int *qt_array;
     * И здесь:  token->qt_array = quote_type;
     * чтобы при экспанде знать, какой символ под single, double или none.
     */
    
    return token;
}


/*
	Static function to create a word token.
	Gathers characters until a space or operator is encountered.
*/
// t_token	*create_word_token(char **input)
// {
// 	char *buf;
// 	char	*start;
// 	int		len;
// 	t_token	*token;

// 	token = malloc(sizeof(t_token));
// 	len = 0;
// 	start = *input;
// 	if (!token)
// 		return (NULL);
// 	token->next = NULL;
// 	while (**input && !ft_isspace(**input) && **input != '|'
// 		&& **input != '<' && **input != '>')
// 	{
// 		len++;
// 		(*input)++;
// 	}
// 	token->value = ft_substr(start, 0, len);
// 	token->type = T_WORD;
// 	return (token);
// }

// char	*create_word_token(char **input)
// {
// 	char	*start;
// 	int		len;

// 	start = *input;
// 	len = 0;
// 	while (**input && !ft_isspace(**input) && **input != '|'
// 		&& **input != '<' && **input != '>')
// 	{
// 		if (**input == '\'' || **input == '\"')
// 		{
// 			ft_putendl_fd("Minishell: syntax error: stray quote",2);
// 			return (NULL);
// 		}
// 		len++;
// 		(*input);
// 	}
// 	return (ft_substr(start, 0, len));
// }