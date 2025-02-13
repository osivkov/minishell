



#include "minishell.h"

t_cmd *parser(t_token *tokens)
{
	t_cmd *cmd_list = NULL;
	t_cmd *current_cmd = NULL;

	// Traverse through the tokens
	while (tokens)
	{
		// Start a new command (for example, if it's the first word or after a pipe)
		if (tokens->type == T_WORD)
		{
			t_cmd *new_cmd = malloc(sizeof(t_cmd));
			// Initialize new_cmd and create an array of arguments
			// Here, you need to collect all T_WORD tokens until the next pipe or the end
			// Example: new_cmd->args = [token values, ...]
			new_cmd->next = NULL;
			// Add the command to the command list
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
		// If the token is a pipe, simply skip it or use it as a command separator
		tokens = tokens->next;
	}
	return (cmd_list);
}