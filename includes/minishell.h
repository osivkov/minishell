/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:38:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/01 11:14:06 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
#define MINISHELL_H


#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif


#ifndef SIGNALS_H
#define SIGNALS_H

/* Режимы для set_signal */
#define STOP_RESTORE   1
#define STOP_QUIT      2
#define EXIT_MODE      3
#define HEREDOC        4
#define HEREDOC_PAUSE  5

/* Прототипы новых обработчиков сигналов */
#endif



#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include "../libft/libft.h"
#include <signal.h>
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

// typedef	enum e_quote_type {
// 	SINGLE_QUOTE,
// 	DOUBLE_QUOTE
// }	t_quote_type;

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
	int				*qt_array;
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
	int		**quote_type;
	// int		parsing_error;// Flag indicating whether the command is built-in
	struct s_cmd	*next;		// Next command in the pipeline (if using pipes)
}	t_cmd;

/* Global variable for signal handling (only one is allowed) */
extern volatile sig_atomic_t g_exit;
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
void	signal_handler(int signum);
void	command_handler(int signum);
void	heredoc_sigint_handler(int sig);
void child_signal_handler(int sig);
void	check_signals(void);
void	ctrl_c(int sig);
void	back_slash(int sig);
void	reset_prompt(int sig);
void	set_signal(int mode, t_minishell *shell);

int		ft_set_env_var(t_minishell *mini, char *key, char *ans);
int		ft_malloc_error(t_minishell *mini);
void	execute(t_minishell *mini);
void	pseudo_execute(t_minishell *mini);
void	expand_command_variables(t_minishell *shell, t_cmd *cmd_list);
char	*get_env_value(t_minishell *shell, const char *var);
/* Function prototypes for parsing */


int			is_operator_char(char c);
t_token		*lexer(t_minishell *shell, char *input);
t_cmd 		*parser(t_minishell *shell, t_token *tokens);
int			handle_heredoc(char *delimeter);
/*list lexer_utils functions*/
int	handle_dollar_quote(t_minishell *shell,
	char **input, char **value, int **qt);
int	handle_single_quote(t_minishell *shell,
	char **input, char **value, int **qt);
int	handle_double_quote(t_minishell *shell,
	char **input, char **value, int **qt);
int	handle_unquoted_char(t_minishell *shell,
	char **input, char **value, int **qt);
int	process_chars(t_minishell *shell, char **input,
	char **value, int **quote_type);
int	process_chars(t_minishell *shell, char **input, char **value, int **quote_type);
char	*build_word_value(t_minishell *shell, char **input, int **out_qt);
int		append_char(char **str, int **qt, char c, int qtype);
void	token_to_list(t_token **head, t_token **current, t_token *new_token);
t_token	*create_double_operator_token(char **input);
t_token	*create_single_operator_token(char **input);
t_token	*create_special_token(char **input);
t_token	*create_word_token(t_minishell *shel,char **input);
// char	**expand_variables(char **args);
/*For expand*/
char	*h_d(t_minishell *shell, const char *str,char *exp, size_t *i);
char	*hane_dol_var(t_minishell *shell, const char *str, size_t *i, char *exp);
char	*append_char_exp(char *expanded, char c);
/*Finish expand*/
/*Function for runn shell*/
t_minishell	*init_minishell(char **env);
int			run_minishell(t_minishell *shell);
/* Function prototypes for shell management */
/* These functions initialize, run, and free the main minishell structure */
void		free_minishell(t_minishell *shell);
void 		free_cmd(t_cmd *cmd);
void 		free_tokens(t_token *tokens);
void		free_args_on_error(char **args, int used);
void		free_quote_types(int **qtypes, int count);
/*Function for utils*/
int			ft_isspace(int c);

/*Function for non interactive shell*/
void	run_noninteractive_minishell(t_minishell *shell, char **argv);

/*Function for executor*/
size_t	strlen_alt(const char *s);
void	ft_free_single(char *s1, char *s2, char *s3, char *s4);
void	ft_free_double(char **s1, char **s2, char **s3, char **s4);
char	*ft_strjoin_all(const char *s1, const char *s2, const char *s3, const char *s4);
int	ft_malloc_error(t_minishell *mini);
void	ft_error_msg(char *command, char *key, char *error_msg);
int	ft_check_env_name(char *name);
char	*ft_get_env_var(t_minishell *mini, char *key);
int	ft_realloc_env(t_minishell *mini, char *temp2);
int	ft_set_env_var(t_minishell *mini, char *key, char *ans);
int	ft_cd_utils(t_minishell *mini, t_cmd *head, char *path, char *pwd);
int	ft_cd(t_minishell *mini, t_cmd *head);
int	ft_echo(t_minishell *mini, t_cmd *head, int new_line, int j);
int	ft_env(t_minishell *mini, t_cmd *head);
int	ft_pwd(t_minishell *mini, t_cmd *head);
int	ft_export_util(t_minishell *mini, char *str, int i);
int	ft_export_util_2(t_minishell *mini);
int	ft_export(t_minishell *mini, t_cmd *head);
int	ft_unset_utils(t_minishell *mini, char *key);
int	ft_unset(t_minishell *mini, t_cmd *head);
int	ft_exit(t_minishell *mini, t_cmd *head);
int	get_final_path(t_minishell *mini, char **all_path, char *path, \
	char *to_find);
int	get_cmd_path(t_minishell *mini, char *to_find, char *path);
int	ft_sys_builtin(t_minishell *mini, t_cmd *head);
int	begin_builtin(t_minishell *mini, t_cmd *head);
int		check_builtin(t_cmd *head);
void	ft_kill_child(t_minishell *mini, int count_cmd);
void	ft_handle_infile(int *fd, t_cmd *head, int i);
void	ft_handle_outfile(int *fd, t_cmd *head, int i);
void	ft_init_child(t_minishell *mini, int *fd, t_cmd *head, int i);
int	*create_pipes(t_minishell *mini, int count_cmd);
int	handle_inout_fd(t_cmd *head);
void	initiate_execute(t_minishell *mini, int *fd, int count_cmd);
void	execute(t_minishell *mini);

#endif