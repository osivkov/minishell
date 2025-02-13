



#include "minishell.h"

// Helper function to parse a single command from tokens.
// It consumes tokens until a T_PIPE or end-of-list is encountered.
static t_cmd *parse_single_command(t_token **tokens)
{
	t_cmd	*cmd;
	int		arg_count;
	char	**args;
	int		i;
	t_token	*runner;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL); // In real code, handle the error appropriately
	cmd->infile = -1;      // Default value: no input redirection
	cmd->outfile = -1;     // Default value: no output redirection
	cmd->is_builtin = 0;   // By default, the command is not built-in
	cmd->next = NULL;

	// Count the number of arguments (T_WORD tokens) until the next pipe or end.
	arg_count = 0;
	runner = *tokens;
	while (runner && runner->type != T_PIPE)
	{
		if (runner->type == T_WORD)
			arg_count++;
		runner = runner->next;
	}

	// Allocate memory for the arguments array (plus a terminating NULL)
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		free(cmd);
		return (NULL);
	}

	i = 0;
	// Fill the arguments array by duplicating token values
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_WORD)
		{
			args[i] = ft_strdup((*tokens)->value);
			i++;
		}
		*tokens = (*tokens)->next;
	}
	args[i] = NULL; // Terminate the array with NULL
	cmd->args = args;

	return (cmd);
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
