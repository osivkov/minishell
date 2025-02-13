
#include "minishell.h"
#include <ctype.h>

t_token *lexer(char *input)
{
	t_token *head = NULL;
	t_token *current = NULL;
	
	while (*input)
	{
		// Skip spaces
		while (*input && isspace(*input))
			input++;
		if (!*input)
			break;
		
		// Create a new token
		t_token *new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (NULL);
		 
		// If a special character is encountered
		if (*input == '|' || *input == '<' || *input == '>')
		{
			// Determine the token type and its value (e.g., "|" or ">" or ">>")
			// For simplicity, handle one character
			new_token->value = ft_strdup((char[]){*input, '\0'});
			if (*input == '|')
				new_token->type = T_PIPE;
			else if (*input == '<')
				new_token->type = T_REDIR_IN;
			else if (*input == '>')
				new_token->type = T_REDIR_OUT;
			input++; // Move to the next character
		}
		else
		{
			// Handle a normal word
			// Gather characters until a space or special character is encountered
			char *start = input;
			while (*input && !isspace(*input) && *input != '|' && *input != '<' && *input != '>')
				input++;
			int len = input - start;
			new_token->value = ft_substr(start, 0, len);
			new_token->type = T_WORD;
		}
		new_token->next = NULL;
		
		// Add the new token to the linked list
		if (!head)
		{
			head = new_token;
			current = new_token;
		}
		else
		{
			current->next = new_token;
			current = new_token;
		}
	}
	return (head);
}