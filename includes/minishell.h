/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:38:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/19 15:01:17 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H




#include <signal.h>
#include "../libft/libft.h"
// Include other standard libraries as needed

/**
 * Enum `e_token_type`
 * Represents different types of tokens that can be found in a shell command.
 * 
 * Each token type corresponds to a specific element in the command syntax.
 * It is used in the lexer and parser to identify different parts of the input.
 */

typedef enum e_token_type {
	T_WORD,			// Word (command or argument)
	T_PIPE,			// Symbol |
	T_REDIR_IN,		// Symbol <
	T_REDIR_OUT,	// Symbol >
	T_REDIR_APPEND,	// Symbol >>
	T_HEREDOC		// Symbol <<
} t_token_type;


/* Token structure */
/* This structure represents a single token extracted from the input.
   It can be a command, argument, or an operator (such as PIPE or REDIRECT). */
typedef struct s_token {
	char			*value; // Token value (command, argument, operator)
	int				type;   // Token type (e.g., COMMAND, ARGUMENT, PIPE, REDIRECT)
	struct s_token	*next;  // Pointer to the next token in the list
} t_token;

/* Command structure */
/* This structure represents a command to be executed.
	It contains an array of arguments, redirection descriptors, and a flag
	indicating if it is a built-in command. The 'next' pointer allows the chaining
	of commands when using pipes. */
typedef struct s_cmd {
	char	**args;		// Argument array (first element is the command)
	int		infile;		// File descriptor for input redirection
	int		outfile;		// File descriptor for output redirection
	int		is_builtin;		// Flag indicating whether the command is built-in
	struct s_cmd	*next;		// Next command in the pipeline (if using pipes)
}	t_cmd;

/* Global variable for signal handling (only one is allowed) */
extern volatile sig_atomic_t g_signal_status;

/* Main Shell structure */
/* This structure holds the global state of the minishell.
	It includes the environment variables, the token list and the command list
	parsed from the input, as well as the last exit status of a command.
	Additional fields (such as command history, settings, etc.) can be added later. */
typedef struct s_minishell {
	char	**env;		// Array of environment variables
	t_token *tokens;		// List of tokens generated from the latest input
	t_cmd	*cmd;		// List of commands parsed from the tokens
	int		last_exit;	// Last command exit status
	// Additional fields can be added here (e.g., history, configuration settings, etc.)
}	t_minishell;

/* Function prototypes for parsing */
t_token	*lexer(char *input);
t_cmd	*parser(t_token *tokens);
char	**expand_variables(char **args);

/* Function prototypes for shell management */
/* These functions initialize, run, and free the main minishell structure */
t_minishell	*init_minishell(char **env);
void		free_minishell(t_minishell *shell);
int			run_minishell(t_minishell *shell);
t_token		*process_quotes(char **input, char quote);
int			handle_heredoc(char *delimeter);

/*Function for free and utils*/
int			ft_isspace(int c);
void 		free_cmd(t_cmd *cmd);
void 		free_tokens(t_token *tokens);
#endif
