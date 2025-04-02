/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_pipline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:58:03 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 17:26:38 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}

void	add_cmd_to_pipeline(t_cmd **cmd_list,
	t_cmd **current_cmd, t_cmd *new_cmd)
{
	if (*cmd_list == NULL)
	{
		*cmd_list = new_cmd;
		*current_cmd = new_cmd;
	}
	else
	{
		(*current_cmd)->next = new_cmd;
		*current_cmd = new_cmd;
	}
}

t_cmd	*parse_pipeline(t_minishell *shell, t_token **tokens)
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
			ft_putendl_fd("minishell: syntax error"
				"near unexpected token `|`", 2);
			shell->last_exit = 2;
			return (free_cmd(cmd_list), NULL);
		}
		new_cmd = parse_command(shell, tokens);
		if (!new_cmd)
			return (free_cmd(cmd_list), NULL);
		add_cmd_to_pipeline(&cmd_list, &current_cmd, new_cmd);
		if (*tokens && (*tokens)->type == T_PIPE)
			*tokens = (*tokens)->next;
		else
			break ;
	}
	return (cmd_list);
}
