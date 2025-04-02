/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:38:15 by osivkov           #+#    #+#             */
/*   Updated: 2025/04/02 10:37:14 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <linux/limits.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include <signal.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096

# endif

# ifndef SIGNALS_H
#  define SIGNALS_H

#  define STOP_RESTORE	1
#  define STOP_QUIT		2
#  define EXIT_MODE		3
#  define HEREDOC		4
#  define HEREDOC_PAUSE	5

# endif

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}	t_token_type;

typedef enum e_parse_err
{
	PARSE_OK,
	PARSE_HEREDOC_ERROR,
	PARSE_NO_FILENAME,
	PARSE_UNCLOSED_QUOTE,
	PARSE_UNKNOWN_TOKEN
}	t_parse_err;

typedef struct s_token
{
	char			*value;
	int				type;
	int				*qt_array;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	int				infile;
	int				outfile;
	int				is_builtin;
	int				**quote_type;
	struct s_cmd	*next;
}	t_cmd;

extern volatile sig_atomic_t	g_exit;
volatile sig_atomic_t			g_exit = 0;
typedef struct s_minishell
{
	char	**env;
	t_token	*tokens;
	t_cmd	*cmd;
	int		last_exit;
}	t_minishell;

typedef struct s_parse_context
{
	t_minishell	*shell;
	t_cmd		*cmd;
	t_token		**tokens;
	char		**args;
	int			**qtypes;
	int			i;
}				t_parse_context;

typedef struct s_heredoc_context
{
	int		backup_fd;
	int		temp_fd;
	void	(*old_handler)(int);
}				t_heredoc_context;

int			run_minishell(t_minishell *shell);
int			process_line(t_minishell *shell, char *input);
char		*get_input(t_minishell *shell);
char		*generate_prompt(t_minishell *shell);
int			process_command(t_minishell *shell, char *command);
t_minishell	*init_minishell(char **env);
char		**dup_env(char **env);
int			count_env(char **env);
void		update_shlvlv(t_minishell *shell);
t_token		*lexer(t_minishell *shell, char *inputs);
t_token		*create_double_operator_token(char **input);
t_token		*create_single_operator_token(char **input);
t_token		*create_special_token(char **input);
t_token		*create_word_token(t_minishell *shel, char **input);
void		signal_handler(int signum);
void		command_handler(int signum);
void		child_signal_handler(int sig);
void		check_signals(void);
void		ctrl_c(int sig);
void		back_slash(int sig);
void		reset_prompt(int sig);
void		set_signal(int mode, t_minishell *shell);
int			ft_set_env_var(t_minishell *mini, char *key, char *ans);
int			ft_malloc_error(t_minishell *mini);
void		execute(t_minishell *mini);
void		pseudo_execute(t_minishell *mini);
void		expand_command_variables(t_minishell *shell, t_cmd *cmd_list);
char		*get_env_value(t_minishell *shell, const char *var);
int			is_operator_char(char c);
t_cmd		*parser(t_minishell *shell, t_token *tokens);
int			handle_heredoc(char *delimeter);
int			is_redirection(t_token_type type);
int			allocate_args(t_minishell *shell,
				int arg_count, char ***args, int ***qtypes);
t_cmd		*parse_command(t_minishell *shell, t_token **tokens);
void		add_cmd_to_pipeline(t_cmd **cmd_list, t_cmd **current_cmd,
				t_cmd *new_cmd);
t_cmd		*parse_pipeline(t_minishell *shell, t_token **tokens);
int			count_args(t_minishell *shell, t_token *runner);
int			validate_and_skip_redirection(t_minishell *shell, t_token **runner);
int			handle_redirect(t_minishell *shell, t_cmd *cmd,
				t_token_type rtype, char *filename);
t_cmd		*alloc_cmd_struct(t_minishell *shell);
int			handle_dollar_quote(t_minishell *shell,
				char **input, char **value, int **qt);
int			handle_single_quote(t_minishell *shell,
				char **input, char **value, int **qt);
int			handle_double_quote(t_minishell *shell,
				char **input, char **value, int **qt);
int			handle_unquoted_char(t_minishell *shell,
				char **input, char **value, int **qt);
int			process_chars(t_minishell *shell, char **input,
				char **value, int **quote_type);
int			process_chars(t_minishell *shell,
				char **input, char **value, int **quote_type);
char		*build_word_value(t_minishell *shell,
				char **input, int **out_qt);
int			append_char(char **str, int **qt, char c, int qtype);
void		token_to_list(t_token **head,
				t_token **current, t_token *new_token);
char		*h_d(t_minishell *shell, const char *str, char *exp, size_t *i);
char		*hane_dol_var(t_minishell *shell, const char *str,
				size_t *i, char *exp);
char		*append_char_exp(char *expanded, char c);
t_minishell	*init_minishell(char **env);
int			run_minishell(t_minishell *shell);
void		free_minishell(t_minishell *shell);
void		free_cmd(t_cmd *cmd);
void		free_tokens(t_token *tokens);
void		free_args_on_error(char **args, int used);
void		free_quote_types(int **qtypes, int count);
int			ft_isspace(int c);
void		run_noninteractive_minishell(t_minishell *shell, char **argv);
size_t		strlen_alt(const char *s);
void		ft_free_single(char *s1, char *s2, char *s3, char *s4);
void		ft_free_double(char **s1, char **s2, char **s3, char **s4);
char		*ft_strjoin_all(const char *s1, const char *s2,
				const char *s3, const char *s4);
int			ft_malloc_error(t_minishell *mini);
void		ft_error_msg(char *command, char *key, char *error_msg);
int			ft_check_env_name(char *name);
char		*ft_get_env_var(t_minishell *mini, char *key);
int			ft_realloc_env(t_minishell *mini, char *temp2);
int			ft_set_env_var(t_minishell *mini, char *key, char *ans);
int			ft_cd_utils(t_minishell *mini, t_cmd *head, char *path, char *pwd);
int			ft_cd(t_minishell *mini, t_cmd *head);
int			ft_echo(t_minishell *mini, t_cmd *head, int new_line, int j);
int			ft_env(t_minishell *mini, t_cmd *head);
int			ft_pwd(t_minishell *mini, t_cmd *head);
int			ft_export_util(t_minishell *mini, char *str, int i);
int			ft_export_util_2(t_minishell *mini);
int			ft_export(t_minishell *mini, t_cmd *head);
int			ft_unset_utils(t_minishell *mini, char *key);
int			ft_unset(t_minishell *mini, t_cmd *head);
int			ft_exit(t_minishell *mini, t_cmd *head);
int			get_final_path(t_minishell *mini, char **all_path, char *path,
				char *to_find);
int			get_cmd_path(t_minishell *mini, char *to_find, char *path);
int			ft_sys_builtin(t_minishell *mini, t_cmd *head);
int			begin_builtin(t_minishell *mini, t_cmd *head);
int			check_builtin(t_cmd *head);
void		ft_kill_child(t_minishell *mini, int count_cmd);
void		ft_handle_infile(int *fd, t_cmd *head, int i);
void		ft_handle_outfile(int *fd, t_cmd *head, int i);
void		ft_init_child(t_minishell *mini, int *fd, t_cmd *head, int i);
int			*create_pipes(t_minishell *mini, int count_cmd);
int			handle_inout_fd(t_cmd *head);
void		initiate_execute(t_minishell *mini, int *fd, int count_cmd);
void		execute(t_minishell *mini);

#endif