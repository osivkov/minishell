/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:39:14 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 17:26:10 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int	allocate_args(t_minishell *shell, int arg_count,
	char ***args, int ***qtypes)
{
	*args = malloc(sizeof(char *) * (arg_count + 1));
	if (!(*args))
	{
		ft_putendl_fd("minishell: syntax error: empty command", 2);
		shell->last_exit = 2;
		return (0);
	}
	*qtypes = malloc(sizeof(int *) * (arg_count + 1));
	if (!(*qtypes))
	{
		ft_putendl_fd("minishell: malloc error", 2);
		free(*args);
		shell->last_exit = 1;
		return (0);
	}
	return (1);
}

t_cmd	*alloc_cmd_struct(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		shell->last_exit = 1;
		ft_putendl_fd("minishell: allocation error", 2);
		return (NULL);
	}
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->is_builtin = 0;
	cmd->next = NULL;
	return (cmd);
}

int	handle_redirect(t_minishell *shell, t_cmd *cmd,
							t_token_type rtype, char *filename)
{
	int	fd;

	fd = -1;
	if (rtype == T_HEREDOC)
	{
		fd = handle_heredoc(filename);
		if (fd == -1)
			return (1);
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

int	validate_and_skip_redirection(t_minishell *shell, t_token **runner)
{
	*runner = (*runner)->next;
	if (!(*runner) || (*runner)->type != T_WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		shell->last_exit = 2;
		return (-1);
	}
	*runner = (*runner)->next;
	return (0);
}

/*
 * Функция count_args:
 * Подсчитывает количество аргументов (T_WORD) до встречаемого T_PIPE.
 * При встрече оператора редиректа вызывает validate_and_skip_redirection,
 * чтобы пропустить оператор и его аргумент.
 */

int	count_args(t_minishell *shell, t_token *runner)
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
			if (validate_and_skip_redirection(shell, &runner) < 0)
				return (-1);
		}
		else
			break ;
	}
	return (count);
}
