/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:13:52 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/20 14:53:11 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>

int		check_builtin(t_cmd *head);
int	ft_exit(t_minishell *mini, t_cmd *head);
/*pipes.c*/
int	*create_pipes(t_minishell *mini, int count_cmd)
{
	int		*fd;
	int		i;

	i = 0;
	fd = ft_calloc(2 * (count_cmd), sizeof(int));
	if (fd == NULL)
	{
		mini->last_exit = errno;
		return (perror("malloc"), NULL);
	}
	while (i < count_cmd)
	{
		if (pipe(fd + (i * 2)) < 0)
		{
			mini->last_exit = errno;
			free (fd);
			return (perror("pipe"), NULL);
		}
		i++;
	}
	return (fd);
}

/*execute_utils.c*/
size_t	strlen_alt(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

/*execute_utils.c*/
//frees all string
void	ft_free_single(char *s1, char *s2, char *s3, char *s4)
{
	if (s1 != NULL)
	{
		free (s1);
		s1 = NULL;
	}
	if (s2 != NULL)
	{
		free (s2);
		s2 = NULL;
	}
	if (s3 != NULL)
	{
		free (s3);
		s3 = NULL;
	}
	if (s4 != NULL)
	{
		free (s4);
		s4 = NULL;
	}
	return ;
}

/*execute_utils.c*/
//frees all 2D char array
void	ft_free_double(char **s1, char **s2, char **s3, char **s4)
{
	size_t	i;

	if (s1 != NULL)
	{
		i = -1;
		while (s1[++i] != NULL)
		{
			free (s1[i]);
			s1[i] = NULL;
		}
		free (s1);
		s1 = NULL;
	}
	if (s2 != NULL)
		ft_free_double(s2, NULL, NULL, NULL);
	if (s3 != NULL)
		ft_free_double(s3, NULL, NULL, NULL);
	if (s4 != NULL)
		ft_free_double(s4, NULL, NULL, NULL);
	return ;
}


/*execute_utils.c*/
//joins all strings. returns NULL in case of malloc failure
char	*ft_strjoin_all(const char *s1, const char *s2, const char *s3, const char *s4)
{
	size_t	len;
	size_t	i;
	char	*final;

	len = strlen_alt(s1) + strlen_alt(s2) + strlen_alt(s3) + strlen_alt(s4);
	final = (char *)ft_calloc(sizeof(char), (len + 1));
	if (final == NULL)
		return (NULL);
	i = -1;
	len = -1;
	while (s1 && s1[++i] != '\0')
		final[++len] = s1[i];
	i = -1;
	while (s2 && s2[++i] != '\0')
		final[++len] = s2[i];
	i = -1;
	while (s3 && s3[++i] != '\0')
		final[++len] = s3[i];
	i = -1;
	while (s4 && s4[++i] != '\0')
		final[++len] = s4[i];
	return (final);
}

/*execute_utils.c*/
//error message
void	ft_error_msg(char *command, char *key, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (key != NULL)
	{
		if (ft_strncmp(command, "export", 7) == 0 || \
			ft_strncmp(command, "unset", 6) == 0)
			ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		if (ft_strncmp(command, "export", 7) == 0 || \
			ft_strncmp(command, "unset", 6) == 0)
			ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(error_msg, STDERR_FILENO);
	return ;
}

/*execute_utils.c*/
//malloc error, will return ENOMEM
int	ft_malloc_error(t_minishell *mini)
{
	mini->last_exit = ENOMEM;
	ft_error_msg("malloc", NULL, strerror(ENOMEM));
	return (ENOMEM);
}

/*env_utils.c*/
int	ft_check_env_name(char *name)
{
	int		i;

	i = 0;
	if (ft_isalpha(name[0]) == 1 || name[0] == '_')
		i++;
	else
		return (1);
	while (name[i] != '\0' && name[i] != '=')
	{
		if (ft_isalnum(name[i]) == 1 || name[i] == '_')
			i++;
		else
			return (1);
	}
	return (0);
}

/*env_utils.c*/
/*will return the value if the key is set
if the key is unset, will return "". 
In case of critical failure, will print malloc error and will return NULL*/
char	*ft_get_env_var(t_minishell *mini, char *key)
{
	int		i;
	char	*temp;
	char	*final;
	int		len;

	i = -1;
	temp = ft_strjoin(key, "=");
	final = ft_strdup("");
	if (temp == NULL || final == NULL)
		return (ft_free_single(temp, final, NULL, NULL), \
			ft_malloc_error(mini), NULL);
	len = ft_strlen(temp);
	while (mini->env[++i] != NULL)
	{
		if (ft_strncmp(mini->env[i], temp, len) == 0)
		{
			ft_free_single(temp, final, NULL, NULL);
			final = ft_substr(mini->env[i], len, ft_strlen(mini->env[i]) - len);
			if (final == NULL)
				return (ft_malloc_error(mini), NULL);
			return (mini->last_exit = 0, final);
		}
	}
	ft_free_single (temp, NULL, NULL, NULL);
	return (mini->last_exit = 0, final);
}

/*env_utils.c*/
//continuation of ft_set_env_var
int	ft_realloc_env(t_minishell *mini, char *temp2)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	while (mini->env[i] != NULL)
		i++;
	new_env = (char **)ft_calloc(sizeof(char *), (i + 2));
	if (new_env == NULL)
		return (ft_malloc_error(mini));
	j = 0;
	while (j < i)
	{
		new_env[j] = mini->env[j];
		j++;
	}
	new_env[j] = temp2;
	new_env[++j] = NULL;
	free (mini->env);
	mini->env = new_env;
	return (0);
}

/*env_utils.c*/
//will return 0 on success
//returns 1 on malloc error, displays malloc error, sets last exit
int	ft_set_env_var(t_minishell *mini, char *key, char *ans)
{
	int		i;
	char	*temp;
	char	*temp2;
	int		len;

	temp = ft_strjoin_all(key, "=", NULL, NULL);
	temp2 = ft_strjoin_all(key, "=", ans, NULL);
	if (temp == NULL || temp2 == NULL)
		return (ft_free_single(temp, temp2, NULL, NULL), ft_malloc_error(mini));
	i = -1;
	len = ft_strlen(temp);
	while (mini->env[++i] != NULL)
	{
		if (ft_strncmp(mini->env[i], temp, len) == 0)
			break ;
	}
	ft_free_single(temp, NULL, NULL, NULL);
	if (mini->env[i] != NULL)
	{
		free(mini->env[i]);
		mini->env[i] = temp2;
		return (0);
	}
	return (ft_realloc_env(mini, temp2));
}

/*builtins.c*/
//returns exit status post resetting env variables
int	ft_cd_utils(t_minishell *mini, t_cmd *head, char *path, char *pwd)
{
	char	*temp;
	int		exit_status;

	temp = ft_get_env_var(mini, "PWD");
	if (temp == NULL)
		return (ENOMEM);
	if (ft_set_env_var(mini, "OLDPWD", temp) != 0)
		return (free (temp), free (path), ENOMEM);
	ft_free_single(path, temp, NULL, NULL);
	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		mini->last_exit = errno;
		ft_error_msg(head->args[0], head->args[1], strerror(errno));
		return (errno);
	}
	temp = ft_strdup(pwd);
	if (temp == NULL)
		return (ft_malloc_error(mini));
	exit_status = ft_set_env_var(mini, "PWD", temp);
	free (temp);
	return (exit_status);
}

/*builtins.c*/
int	ft_cd(t_minishell *mini, t_cmd *head)
{
	char	*path;
	char	pwd[PATH_MAX];

	if (head->args[1] != NULL && head->args[2] != NULL)
	{
		ft_error_msg(head->args[0], NULL, "too many arguments");
		return (1);
	}
	if (head->args[1] == NULL || head->args[1][0] == '\0' || ft_strncmp(head->args[1], "~", 2) == 0)
		path = ft_get_env_var(mini, "HOME");
	else if (ft_strncmp(head->args[1], "-", 2) == 0)
		path = ft_get_env_var(mini, "OLDPWD");
	else
		path = ft_strdup(head->args[1]);
	if (path == NULL)
		return (ft_malloc_error(mini));
	if (chdir(path) != 0)
	{
		ft_free_single(path, NULL, NULL, NULL);
		ft_error_msg(head->args[0], head->args[1], strerror(errno));
		return (errno);
	}
	return (ft_cd_utils(mini, head, path, pwd));
}

/*builtins.c*/
//new_line and d_flag is set to 0
int	ft_echo(t_minishell *mini, t_cmd *head, int new_line, int d_flag)
{
	int		i;

	if (head->args[1] != NULL && ft_strncmp((head->args[1]), "-n", 2) == 0)
	{
		i = 2;
		while (head->args[1][i] != '\0' && head->args[1][i] == 'n')
			i++;
		if (i == 2 && head->args[1][i] == '\0')
			new_line = 1;
		else if (i > 2 && head->args[1][i] == '\0')
			d_flag = 1;
	}
	i = 1 + d_flag + new_line;
	while (head->args[i] != NULL)
	{
		printf("%s", head->args[i]);
		i++;
		if (head->args[i] != NULL)
			printf(" ");
	}
	if (new_line != 1 && d_flag != 1)
		printf("\n");
	mini->last_exit = 0;
	return (0);
}

/*builtins.c*/
int	ft_env(t_minishell *mini, t_cmd *head)
{
	int		i;

	i = 0;
	if (head->args[1] != NULL)
	{
		mini->last_exit = 1;
		return (ft_error_msg(head->args[0], NULL, "too many arguments"), 1);
	}
	i = 0;
	while (mini->env[i] != NULL)
	{
		printf("%s\n", mini->env[i]);
		i++;
	}
	mini->last_exit = 0;
	return (0);
}

/*builtins.c*/
int	ft_pwd(t_minishell *mini, t_cmd *head)
{
	char	pwd[PATH_MAX];

	if (head->args[1] != NULL)
	{
		mini->last_exit = 1;
		return (ft_error_msg(head->args[0], NULL, "too many arguments"), 1);
	}
	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		mini->last_exit = errno;
		return (ft_error_msg(head->args[0], head->args[1], \
			strerror(errno)), errno);
	}
	printf("%s\n", pwd);
	mini->last_exit = 0;
	return (0);
}

/*builtins.c*/
//i = 0. returns 1 in case of malloc error
int	ft_export_util(t_minishell *mini, char *str, int i)
{
	char	*value;
	char	**new_env;

	while (str[i] != '\0' && str[i] != '=')
		i++;
	if (str[i] == '\0')
		return (0);
	i = 0;
	while (mini->env[i] != NULL)
		i++;
	value = ft_strdup(str);
	if (value == NULL)
		return (1);
	new_env = (char **)ft_calloc(sizeof(char *), (i + 2));
	if (new_env == NULL)
		return (free (value), 1);
	i = -1;
	while (mini->env[++i] != NULL)
		new_env[i] = mini->env[i];
	new_env[i] = value;
	new_env[i + 1] = NULL;
	free (mini->env);
	mini->env = new_env;
	return (0);
}


/*builtins.c*/
int	ft_export(t_minishell *mini, t_cmd *head)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	while (head->args[++i] != NULL)
	{
		if (ft_check_env_name(head->args[i]) == 1)
		{
			exit_status = 1;
			ft_error_msg("export", head->args[i], "not a valid identifier");
		}
		else
		{
			if (ft_export_util(mini, head->args[i], 0) == 1)
				exit_status = ft_malloc_error(mini);
		}
	}
	return (0);
}


/*builtins.c*/
//returns 1 in case of malloc failure. will set last error and display error msg
int	ft_unset_utils(t_minishell *mini, char *key)
{
	int		i;
	int		len;
	char	*temp;

	i = -1;
	temp = ft_strjoin(key, "=");
	if (temp == NULL)
		return (ft_malloc_error(mini));
	len = ft_strlen(temp);
	while (mini->env[++i] != NULL)
	{
		if (ft_strncmp(mini->env[i], temp, len) == 0)
			break ;
	}
	if (mini->env[i] == NULL)
		return (free (temp), 0);
	ft_free_single (mini->env[i], temp, NULL, NULL);
	while (mini->env[++i] != NULL)
		mini->env[i - 1] = mini->env[i];
	mini->env[i - 1] = NULL;
	return (0);
}

/*builtins.c*/
int	ft_unset(t_minishell *mini, t_cmd *head)
{
	int		i;

	i = 1;
	while (head->args[i] != NULL)
	{
		if (ft_unset_utils(mini, head->args[i]) != 0)
			return (0);
		i++;
	}
	return (0);
}

/*ft_sys_builtin.c*/
/*will write the cmd path into path and free temp path and return 0
if path is not found, it will print error message and return errno*/
int	get_final_path(t_minishell *mini, char **all_path, char *path, \
	char *to_find)
{
	char	*full_path;
	int		i;
	int		j;

	i = -1;
	while (all_path[++i] != NULL)
	{
		full_path = ft_strjoin_all(all_path[i], "/", to_find, NULL);
		if (full_path == NULL)
		{
			ft_free_double(all_path, NULL, NULL, NULL);
			ft_free_single(full_path, NULL, NULL, NULL);
			return (ft_malloc_error(mini), ENOMEM);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			j = -1;
			while (full_path[++j] != '\0')
				path[j] = full_path[j];
			path[j] = '\0';
			ft_free_double(all_path, NULL, NULL, NULL);
			ft_free_single(full_path, NULL, NULL, NULL);
			return (0);
		}
	}
	printf("path error\n");
	ft_free_double(all_path, NULL, NULL, NULL);
	ft_free_single(full_path, NULL, NULL, NULL);
	return (ft_error_msg(to_find, NULL, strerror(127)), 127);
}

/*ft_sys_builtin.c*/
/*will return 0 if cmd path is found and wil write it into path
if cmd path is not found, it will print error message and return errno*/
int	get_cmd_path(t_minishell *mini, char *to_find, char *path)
{
	char	*temp;
	char	**all_path;

	temp = ft_get_env_var(mini, "PATH");
	if (temp == NULL)
		return (ENOMEM);
	else if (ft_strncmp(temp, "", 1) == 0)
	{
		printf("minishell: %s: No such file or directory\n", to_find);
		return (free (temp), 127);
	}
	all_path = ft_split(temp, ':');
	ft_free_single(temp, NULL, NULL, NULL);
	if (all_path == NULL)
		return (ft_malloc_error(mini), ENOMEM);
	return (get_final_path(mini, all_path, path, to_find));
}

/*ft_sys_builtin.c*/
int	ft_sys_builtin(t_minishell *mini, t_cmd *head)
{
	char	path[PATH_MAX];
	int		exit_status;
	int		i;

	exit_status = 0;
	if (access(head->args[0], F_OK | X_OK) == 0)
	{
		i = -1;
		while (head->args[0][++i] != '\0')
			path[i] = head->args[0][i];
		path[i] = '\0';
	}
	else
	{
		exit_status = get_cmd_path(mini, head->args[0], path);
		if (exit_status != 0)
			return (exit_status);
	}
	if (execve(path, head->args, mini->env) == -1)
	{
		mini->last_exit = errno;
		return (ft_error_msg(head->args[0], NULL, strerror(errno)), errno);
	}
	return (exit_status);
}


/*builtins.c*/
/*if it is built in and is the only cmd
	->it returns the exit_status of the command
else it exits with the exit status
*/
int	begin_builtin(t_minishell *mini, t_cmd *head)
{
	int		exit_status;

	exit_status = 0;
	if (ft_strncmp(head->args[0], "cd", 3) == 0)
		exit_status = ft_cd(mini, head);
	else if (ft_strncmp(head->args[0], "echo", 5) == 0)
		exit_status = ft_echo(mini, head, 0, 0);
	else if (ft_strncmp(head->args[0], "env", 4) == 0)
		exit_status = ft_env(mini, head);
	else if (ft_strncmp(head->args[0], "export", 7) == 0)
		exit_status = ft_export(mini, head);
	else if (ft_strncmp(head->args[0], "pwd", 4) == 0)
		exit_status = ft_pwd(mini, head);
	else if (ft_strncmp(head->args[0], "unset", 6) == 0)
		exit_status = ft_unset(mini, head);
	else if (ft_strncmp(head->args[0], "exit", 5) == 0)
		exit_status = ft_exit(mini, head);
	else
		exit_status = ft_sys_builtin(mini, head);
	if (mini->cmd->next == NULL && check_builtin(head) == 0)
		return (exit_status);
	exit (exit_status);
	return (exit_status);
}

/*execute.c*/
//resets infile and outfile in cmd structs to to 0 and 1 if there is no redirection
int	handle_inout_fd(t_cmd *head)
{
	int		count_cmd;

	count_cmd = 0;
	while (head != NULL)
	{
		if (head->infile < 0)
			head->infile = STDIN_FILENO;
		if (head->outfile < 0)
			head->outfile = STDOUT_FILENO;
		count_cmd++;
		head = head->next;
	}
	return (count_cmd);
}

/*execute.c*/
void	ft_clean_execute(t_minishell *mini)
{
	t_cmd	*next;

	while (mini->cmd != NULL)
	{
		next = mini->cmd->next;
		ft_free_double(mini->cmd->args, NULL, NULL, NULL);
		free (mini->cmd);
		mini->cmd = next;
	}
}

/*builtins.c*/
//to be modified
int	ft_exit(t_minishell *mini, t_cmd *head)
{
	if (head->args[1] != NULL)
	{
		ft_error_msg(head->args[0], NULL, "too many arguments");
		return (1);
	}
	free_minishell(mini);
	exit (0);
}

/*execute.c*/
//waits for child process and updates last exit in t_minishell
void	ft_kill_child(t_minishell *mini, int count_cmd)
{
	int		i;
	pid_t	child_pid;
	int		status;

	i = 0;
	while (i < count_cmd)
	{
		child_pid = wait (&status);
		if (WIFEXITED(status))
			mini->last_exit = WEXITSTATUS(status);
		i++;
	}
}

/*execute.c*/
//duplicates infile fd in pipes communication
void	ft_handle_infile(int *fd, t_cmd *head, int i)
{
	if (head->infile != STDIN_FILENO)
	{
		if (dup2(head->infile, STDIN_FILENO) == -1)
		{
			perror("dup2 (file-in)");
			exit (1);			
		}
		close (head->infile);
	}
	else if (i != 0)
	{
		if (dup2(fd[i - 2], STDIN_FILENO) == -1)
		{
			perror("dup2 (pipe-in)");
			exit (1);
		}
	}
}

/*execute.c*/
//duplicates outfile fd in pipes communication
void	ft_handle_outfile(int *fd, t_cmd *head, int i)
{
	if (head->outfile != STDOUT_FILENO)
	{
		if (dup2(head->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2 (file-out)");
			exit (1);
		}
		close (head->outfile);
	}
	else if (head->next != NULL)
	{
		if (dup2(fd[i + 1], STDOUT_FILENO) == -1)
		{
			perror("dup2 (pipe-out)");
			exit (1);
		}
	}
}

/*execute.c*/
//initiates child process
void	ft_init_child(t_minishell *mini, int *fd, t_cmd *head, int i)
{
	int		count_cmd;
	t_cmd	*temp;
	int		j;

	temp = mini->cmd;
	count_cmd = 0;
	while (temp != NULL)
	{
		count_cmd++;
		temp = temp->next;
	}
	ft_handle_infile(fd, head, i);
	ft_handle_outfile(fd, head, i);
	j = -1;
	while (++j < (2 * count_cmd))
		close(fd[j]);
	begin_builtin(mini, head);
}

/*execute.c*/
//begins execution.
void	initiate_execute(t_minishell *mini, int *fd, int count_cmd)
{
	t_cmd	*head;
	pid_t	pid;
	int		i;

	head = mini->cmd;
	i = 0;
	while (head != NULL)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("pipe");
			exit (1);
		}
		if (pid == 0)
			ft_init_child(mini, fd, head, i);
		i = i + 2;
		head = head->next;
	}
	i = -1;
	while (++i < (2 * count_cmd))
		close (fd[i]);
	ft_kill_child(mini, count_cmd);
}

/*execute.c*/
//returns 0 if it is a builtin command, else it returns 1
int		check_builtin(t_cmd *head)
{
	if (ft_strncmp(head->args[0], "cd", 3) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "echo", 5) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "env", 4) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "export", 7) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "pwd", 4) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "unset", 6) == 0)
		return (0);
	else if (ft_strncmp(head->args[0], "exit", 5) == 0)
		return (0);
	return (1);
}

/*execute.c*/
/*will execute all commands in mini->cmd, will set last_exit
will free cmd and args in cmd and then return*/
void	execute(t_minishell *mini)
{
	int		count_cmd;
	int		*fd;

	count_cmd = handle_inout_fd(mini->cmd);
	fd = NULL;
	if (count_cmd == 0)
		return ;
	if (count_cmd == 1 && check_builtin(mini->cmd) == 0)
	{
		mini->last_exit = begin_builtin(mini, mini->cmd);
		return ;
	}
	fd = create_pipes(mini, count_cmd);
	if (fd != NULL)
	{
		initiate_execute(mini, fd, count_cmd);
		free (fd);
	}
	// ft_clean_execute(mini);
	return ;
}
