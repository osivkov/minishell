/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:38:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/12 13:57:47 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "../libft/libft.h"

// Include other standard libraries as needed

/* Token structure */
typedef struct s_token {
	char			*value;	// Token value (command, argument, operator)
	int				type;	// Token type (e.g., COMMAND, ARGUMENT, PIPE, REDIRECT)
	struct s_token  *next;
} t_token;

/* Command structure */
typedef struct s_cmd	{
	char			**args;		// Argument array (first element is the command)
	int				infile;		// File descriptor for input redirection
	int				outfile;	// File descriptor for output redirection
	int				is_builtin;// Flag indicating whether the command is a built-in
	struct s_cmd	*next;		// Next command in the pipeline (if using pipes)
}	t_cmd;

/* Global variable for signal handling (only one is allowed) */
extern volatile sig_atomic_t g_signal_status;

/* Function prototypes for parsing */
t_token	*lexer(char *input);
t_cmd	*parser(t_token *tokens);
char	**expand_variables(char **args);

#endif
