/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:38:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/28 16:51:31 by osivkov          ###   ########.fr       */
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

typedef enum	e_token_type {
	T_WORD,			// Word (command or argument)
	T_PIPE,			// Symbol |
	T_REDIR_IN,		// Symbol <
	T_REDIR_OUT,	// Symbol >
	T_REDIR_APPEND,	// Symbol >>
	T_HEREDOC		// Symbol <<
} t_token_type;

typedef enum	e_parse_err {
	PARSE_OK,
	PARSE_HEREDOC_ERROR,
	PARSE_NO_FILENAME,
	PARSE_UNCLOSED_QUOTE,
	PARSE_UNKNOWN_TOKEN
}	t_parse_err; 

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
	int		is_builtin;
	// int		parsing_error;// Flag indicating whether the command is built-in
	struct s_cmd	*next;		// Next command in the pipeline (if using pipes)
}	t_cmd;

/* Global variable for signal handling (only one is allowed) */
extern volatile sig_atomic_t g_signal_status;
void	handle_sigint(int sig);

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

int execute(t_minishell *shell, t_cmd *cmd);
void	expand_command_variables(t_minishell *shell, t_cmd *cmd_list);
char	*get_env_value(t_minishell *shell, const char *var);
/* Function prototypes for parsing */
t_token		*lexer(t_minishell *shell, char *input);
t_cmd 		*parser(t_minishell *shell, t_token *tokens);
t_token 	*process_quotes(t_minishell *shell, char **input, char quote_char);
int			handle_heredoc(char *delimeter);
/*list lexer_utils functions*/
void	token_to_list(t_token **head, t_token **current, t_token *new_token);
t_token	*create_double_operator_token(char **input);
t_token	*create_single_operator_token(char **input);
t_token	*create_special_token(char **input);
t_token	*create_word_token(char **input);
// char	**expand_variables(char **args);

/*Function for runn shell*/
t_minishell	*init_minishell(char **env);
int			run_minishell(t_minishell *shell);
/* Function prototypes for shell management */
/* These functions initialize, run, and free the main minishell structure */
void		free_minishell(t_minishell *shell);
void 		free_cmd(t_cmd *cmd);
void 		free_tokens(t_token *tokens);

/*Function for utils*/
int			ft_isspace(int c);

#endif
