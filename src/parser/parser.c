/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:32 by osivkov           #+#    #+#             */
/*   Updated: 2025/03/28 17:52:00 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static void free_quote_types(int **qtypes, int count)
{
	int i;

	if (!qtypes)
		return;
	for (i = 0; i < count; i++)
	{
		if (qtypes[i])
		free(qtypes[i]);
	}
	free(qtypes);
}



static void	free_args_on_error(char **args, int used)
{
	while (--used >= 0)
		free(args[used]);
	free(args);
}

static t_cmd	*alloc_cmd_struct(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		shell->last_exit = 2;
		ft_putendl_fd("minishell: allocation error", 2);
		return (NULL);
	}
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->is_builtin = 0;
	cmd->next = NULL;
	return (cmd);
}


static int	count_args(t_minishell *shell, t_token *runner)
{
	int	count;

	count = 0;
	while (runner && runner->type != T_PIPE)
	{
		if (runner->type == T_WORD)
		{
			count++;
			runner = runner->next;
		}
		else if (runner->type == T_REDIR_IN || runner->type == T_REDIR_OUT
			|| runner->type == T_REDIR_APPEND || runner->type == T_HEREDOC)
		{
			runner = runner->next;
			if (!runner || runner->type != T_WORD)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token",
					2);
				shell->last_exit = 2;
				return (-1);
			}
			runner = runner->next;
		}
		else
			break ;
	}
	return (count);
}

static int	handle_redirect(t_minishell *shell, t_cmd *cmd,
							t_token_type rtype, char *filename)
{
	int	fd;

	fd = -1;
	if (rtype == T_HEREDOC)
	{
		fd = handle_heredoc(filename);
		
		if (fd == -1)
		{
			return (1);
		}
		cmd->infile = fd;
	}
	else if (rtype == T_REDIR_IN)
		cmd->infile = open(filename, O_RDONLY);
	else if (rtype == T_REDIR_OUT)
		cmd->outfile = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (rtype == T_REDIR_APPEND)
		cmd->outfile = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (rtype != T_HEREDOC && cmd->infile < 0 && cmd->outfile < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(filename);
		shell->last_exit = 1;
		return (1);
	}
	return (0);
}

/*
** parse_command:
**  Парсит одну команду до PIPE, выделяя аргументы (T_WORD)
**  и обрабатывая редиректы (в том числе heredoc).
*/
static t_cmd	*parse_command(t_minishell *shell, t_token **tokens)
{
	t_cmd	*cmd;
	int		arg_count;
	char	**args;
	int		**qtypes; // Здесь будем хранить указатели на массивы кавычек для каждого аргумента
	int		i;

	i = 0;
	cmd = alloc_cmd_struct(shell);
	if (!cmd)
		return (NULL);
	arg_count = count_args(shell, *tokens);
	if (arg_count < 0)
		return (free(cmd), NULL);
	// Выделяем память для массива аргументов
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (free(cmd), shell->last_exit = 2, NULL);
	// Выделяем память для массива указателей на qt_array для каждого аргумента
	qtypes = malloc(sizeof(int *) * (arg_count + 1));
	if (!qtypes)
	{
		free(args);
		free(cmd);
		free_quote_types(qtypes, i);
		shell->last_exit = 10;
		return (NULL);
	}
	/* Обрабатываем все токены до T_PIPE */
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_WORD)
	{
    	// Дублируем значение токена
    	args[i] = ft_strdup((*tokens)->value);
    	// Переносим указатель на массив с информацией о кавычках
    	qtypes[i] = (*tokens)->qt_array;
    	// Обнуляем qt_array в токене, чтобы избежать двойного освобождения
    	(*tokens)->qt_array = NULL;
    	i++;
    	*tokens = (*tokens)->next;
	}
		else if ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_REDIR_OUT
			|| (*tokens)->type == T_REDIR_APPEND || (*tokens)->type == T_HEREDOC)
		{
			t_token_type	rtype = (*tokens)->type;

			*tokens = (*tokens)->next;
			if (!(*tokens) || (*tokens)->type != T_WORD)
			{
				ft_putendl_fd("minishell: syntax error near operator", 2);
				free_args_on_error(args, i);
				free_quote_types(qtypes, i);
				free(cmd);
				shell->last_exit = 2;
				return (NULL);
			}
			if (handle_redirect(shell, cmd, rtype, (*tokens)->value))
			{
				free_args_on_error(args, i);
				free_quote_types(qtypes, i);
				free(cmd);
				return (NULL);
			}
			*tokens = (*tokens)->next;
		}
		else
			break ;
	}
	args[i] = NULL;
	qtypes[i] = NULL;
	cmd->args = args;
	cmd->quote_type = qtypes;
	return (cmd);
}


/*
** parse_pipeline:
**  Собирает несколько команд, разделённых PIPE, в связанный список.
*/
static t_cmd	*parse_pipeline(t_minishell *shell, t_token **tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == T_PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|`", 2);
			free_cmd(cmd_list);
			shell->last_exit = 2;
			return (NULL);
		}
		new_cmd = parse_command(shell, tokens);
		if (!new_cmd)
		{
			free_cmd(cmd_list);
			return (NULL);
		}
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
		if (*tokens && (*tokens)->type == T_PIPE)
			*tokens = (*tokens)->next;
		else
			break ;
	}
	return (cmd_list);
}

/*
** parser:
**  Точка входа, возвращает связанный список команд.
*/
t_cmd	*parser(t_minishell *shell, t_token *tokens)
{
	t_cmd	*result;

	result = parse_pipeline(shell, &tokens);
	return (result);
}