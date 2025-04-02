/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:32 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 17:21:07 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static int	process_word_token(t_parse_context *ctx)
{
	ctx->args[ctx->i] = ft_strdup((*ctx->tokens)->value);
	ctx->qtypes[ctx->i] = (*ctx->tokens)->qt_array;
	(*ctx->tokens)->qt_array = NULL;
	ctx->i++;
	*ctx->tokens = (*ctx->tokens)->next;
	return (1);
}

/* Обработка редиректов */
static int	process_redirection_token(t_parse_context *ctx)
{
	t_token_type	rtype;

	rtype = (*ctx->tokens)->type;
	*ctx->tokens = (*ctx->tokens)->next;
	if (!(*ctx->tokens) || (*ctx->tokens)->type != T_WORD)
	{
		ft_putendl_fd("minishell: syntax error near operator", 2);
		free_args_on_error(ctx->args, ctx->i);
		free_quote_types(ctx->qtypes, ctx->i);
		free(ctx->cmd);
		ctx->shell->last_exit = 2;
		return (0);
	}
	if (handle_redirect(ctx->shell, ctx->cmd, rtype, (*ctx->tokens)->value))
	{
		free_args_on_error(ctx->args, ctx->i);
		free_quote_types(ctx->qtypes, ctx->i);
		free(ctx->cmd);
		return (0);
	}
	*ctx->tokens = (*ctx->tokens)->next;
	return (1);
}

/* Итерация по токенам до T_PIPE и заполнение массивов */
static int	process_tokens(t_parse_context *ctx)
{
	while (*ctx->tokens && (*ctx->tokens)->type != T_PIPE)
	{
		if ((*ctx->tokens)->type == T_WORD)
		{
			if (!process_word_token(ctx))
				return (0);
		}
		else if (is_redirection((*ctx->tokens)->type))
		{
			if (!process_redirection_token(ctx))
				return (0);
		}
		else
			break ;
	}
	ctx->args[ctx->i] = NULL;
	ctx->qtypes[ctx->i] = NULL;
	return (1);
}

/* Основная функция, собирающая команду */
t_cmd	*parse_command(t_minishell *shell, t_token **tokens)
{
	t_cmd			*cmd;
	int				arg_count;
	char			**args;
	int				**qtypes;
	t_parse_context	ctx;

	cmd = alloc_cmd_struct(shell);
	if (!cmd)
		return (NULL);
	arg_count = count_args(shell, *tokens);
	if (arg_count < 0)
		return (free(cmd), NULL);
	if (!allocate_args(shell, arg_count, &args, &qtypes))
		return (free(cmd), NULL);
	ctx.shell = shell;
	ctx.cmd = cmd;
	ctx.tokens = tokens;
	ctx.args = args;
	ctx.qtypes = qtypes;
	ctx.i = 0;
	if (!process_tokens(&ctx))
		return (NULL);
	cmd->args = args;
	cmd->quote_type = qtypes;
	return (cmd);
}

t_cmd	*parser(t_minishell *shell, t_token *tokens)
{
	t_cmd	*result;

	result = parse_pipeline(shell, &tokens);
	return (result);
}
