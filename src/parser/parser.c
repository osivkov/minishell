/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:32 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 14:42:31 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static t_cmd *parse_single_command(t_minishell *shell, t_token **tokens)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        fprintf(stderr, "minishell: allocation error\n");
        shell->last_exit = 2;  // или 1, в зависимости от вашей логики
        return NULL;
    }
    cmd->infile = -1;
    cmd->outfile = -1;
    cmd->is_builtin = 0;
    cmd->next = NULL;

    // Подсчитываем количество "слов" (аргументов)
    int arg_count = 0;
    t_token *runner = *tokens;
    while (runner && runner->type != T_PIPE)
    {
        if (runner->type == T_WORD)
            arg_count++;
        else if (runner->type == T_REDIR_IN ||
                 runner->type == T_REDIR_OUT ||
                 runner->type == T_REDIR_APPEND ||
                 runner->type == T_HEREDOC)
        {
            // Здесь мы «пропускаем» оператор и следующее слово (имя файла).
            runner = runner->next; // пропускаем оператор
            if (!runner || runner->type != T_WORD)
            {
                // Значит нет аргумента после оператора или это не WORD
                fprintf(stderr,
                    "minishell: syntax error near unexpected token `%s`\n",
                    (runner && runner->value) ? runner->value : "newline");
                shell->last_exit = 2;
                free(cmd);
                return NULL;
            }
            // Само слово не считается аргументом команды (это имя файла), значит arg_count не увеличиваем
            runner = runner->next;
            continue;
        }
        runner = runner->next;
    }

    // Если у нас нет ни одного WORD, то это может быть пустая «команда»
    if (arg_count == 0)
    {
        // Можно вернуть какую-то ошибку или (как вариант) разрешить пустые команды
        // Для примера считаем это ошибкой:
        fprintf(stderr, "minishell: syntax error: empty command\n");
        shell->last_exit = 2;
        free(cmd);
        return NULL;
    }

    // Выделяем массив аргументов
    char **args = malloc(sizeof(char*) * (arg_count + 1));
    if (!args)
    {
        fprintf(stderr, "minishell: allocation error\n");
        shell->last_exit = 2;
        free(cmd);
        return NULL;
    }
    int i = 0;

    // Теперь идём по списку ещё раз, но уже реально заполняем cmd
    while (*tokens && (*tokens)->type != T_PIPE)
    {
        if ((*tokens)->type == T_WORD)
        {
            args[i] = ft_strdup((*tokens)->value);
            i++;
        }
        else if ((*tokens)->type == T_REDIR_IN ||
                 (*tokens)->type == T_REDIR_OUT ||
                 (*tokens)->type == T_REDIR_APPEND ||
                 (*tokens)->type == T_HEREDOC)
        {
            t_token_type redir_type = (*tokens)->type;
            *tokens = (*tokens)->next; // пропускаем сам оператор

            // Должен быть T_WORD после оператора
            if (!(*tokens) || (*tokens)->type != T_WORD)
            {
                // Уже проверили выше, но на всякий случай
                fprintf(stderr, "minishell: syntax error near operator\n");
                shell->last_exit = 2;
                // Освободим уже накопленные args
                while (--i >= 0)
                    free(args[i]);
                free(args);
                free(cmd);
                return NULL;
            }

            // Имеем имя файла (или делимитер для heredoc)
            if (redir_type == T_HEREDOC)
            {
                // handle_heredoc — ваша функция
                int heredoc_fd = handle_heredoc((*tokens)->value);
                if (heredoc_fd == -1)
                {
                    // handle_heredoc сам может вывести ошибку
                    shell->last_exit = 1; // или что-то подходящее
                    while (--i >= 0)
                        free(args[i]);
                    free(args);
                    free(cmd);
                    return NULL;
                }
                cmd->infile = heredoc_fd;
            }
            else if (redir_type == T_REDIR_IN)
                cmd->infile = open((*tokens)->value, O_RDONLY);
            else if (redir_type == T_REDIR_OUT)
                cmd->outfile = open((*tokens)->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            else if (redir_type == T_REDIR_APPEND)
                cmd->outfile = open((*tokens)->value, O_CREAT | O_WRONLY | O_APPEND, 0644);

            *tokens = (*tokens)->next; // пропускаем имя файла
        }
        else
        {
            // Теоретически сюда попасть не должны (T_PIPE или др. операторы), выходим
            break;
        }
        if (*tokens)
            *tokens = (*tokens)->next;
        else
            break;
    }

    args[i] = NULL;
    cmd->args = args;

    // Если infile/outfile не были установлены, по умолчанию можно считать -1
    // или поставить infile = STDIN_FILENO, outfile = STDOUT_FILENO

    return cmd;
}



// Main parser function that processes the entire token list into a command list.
t_cmd *parser(t_minishell *shell, t_token *tokens)
{
    t_cmd   *cmd_list = NULL;
    t_cmd   *current_cmd = NULL;
    t_cmd   *new_cmd;

    while (tokens)
    {
        // Пример простой проверки: если первый токен pipe => синтаксическая ошибка
        if (tokens->type == T_PIPE)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `|`\n");
            shell->last_exit = 2;
            free_cmd(cmd_list);
            return NULL;
        }

        new_cmd = parse_single_command(shell, &tokens);
        if (!new_cmd)
        {
            // Если parse_single_command вернул NULL => была ошибка
            // parser завершает разбор; освобождаем уже созданный список команд
            free_cmd(cmd_list);
            return NULL;
        }

        // Добавляем команду в список
        if (!cmd_list)
        {
            cmd_list = new_cmd;
            current_cmd = new_cmd;
        }
        else
        {
            current_cmd->next = new_cmd;
            current_cmd = new_cmd;
        }

        // Если парсер "остановился" на пайпе — пропустим токен-PIPE и идём дальше
        if (tokens && tokens->type == T_PIPE)
            tokens = tokens->next;
    }

    return cmd_list;
}

