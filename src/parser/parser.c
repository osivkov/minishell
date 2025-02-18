/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 10:21:32 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/18 14:23:17 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// Helper function to parse a single command from tokens.
// It consumes tokens until a T_PIPE or end-of-list is encountered.
static t_cmd *parse_single_command(t_token **tokens)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;
	// Initialize fields
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->is_builtin = 0;
	cmd->next = NULL;

	// Count the number of arguments
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
			// This token is not an argument itself.
			// We skip the operator and ignore the next T_WORD in arg_count.
			runner = runner->next; // skip the operator
			if (runner && runner->type == T_WORD)
			{
				// Just do not count it in arg_count
				runner = runner->next;
			}
			continue;
		}
		runner = runner->next;
	}

	// Allocate memory for the arguments array
	char **args = malloc(sizeof(char*) * (arg_count + 1));
	if (!args)
	{
		free(cmd);
		return NULL;
	}
	int i = 0;

	// Fill the arguments array
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
			*tokens = (*tokens)->next; // skip the operator

			if (*tokens && (*tokens)->type == T_WORD)
			{
				// File or document name
				if (redir_type == T_REDIR_IN)
					cmd->infile = open((*tokens)->value, O_RDONLY);
				else if (redir_type == T_REDIR_OUT)
					cmd->outfile = open((*tokens)->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				// Similarly for T_REDIR_APPEND, T_HEREDOC
				*tokens = (*tokens)->next;
				continue;
			}
			else
			{
				// Error: no filename after redirection
				// Handle syntax error here
				free(args);
				free(cmd);
				return NULL;
			}
		}
		else
		{
			// Skip other unexpected tokens or treat them as errors
			*tokens = (*tokens)->next;
			continue;
		}
		*tokens = (*tokens)->next;
	}

	args[i] = NULL;
	cmd->args = args;
	return cmd;
}


// Main parser function that processes the entire token list into a command list.
t_cmd *parser(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;

	cmd_list = NULL;
	current_cmd = NULL;

	while (tokens)
	{
		// If a T_PIPE token is encountered, skip it.
		if (tokens->type == T_PIPE)
		{
			tokens = tokens->next;
			continue;
		}

		// Parse a single command from the tokens.
		new_cmd = parse_single_command(&tokens);
		if (!new_cmd)
		{
			// In a complete implementation, free previously allocated commands here.
			return (NULL);
		}

		// Add the new command to the command list.
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
	}
	return (cmd_list);
}
