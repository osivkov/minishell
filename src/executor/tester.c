// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   tester.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/03 09:52:30 by marvin            #+#    #+#             */
// /*   Updated: 2025/03/07 13:04:05 by osivkov          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */


// /*suggestions to be implemented later

// manually enter malloc error to 12 instead of errno as not always errno is thrown for malloc error
// Error message: "malloc: cannot allocate memory"
// */

// #include "minishell.h"
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <errno.h>

// /*Start of ft_env_utils.c*/

// void	ft_error_msg(char *command, char *key, char *error_msg);
// void	ft_terminate_execute(t_minishell *mini);
// char	*ft_get_env_var(t_minishell *mini, char *key);
// char	*ft_join_strs(char *s1, char *s2, char *s3, char *s4);

// int	ft_key_len(t_minishell *mini, char *key)
// {
// 	int		i;

// 	// printf("key_len fnc, key: %s\n", key);
// 	i = 0;
// 	if (mini == NULL)
// 		return (0);
// 	if (ft_isalpha(key[0]) == 0 && key[0] != '_')
// 		return (0);
// 	while (key[i] != '\0' && (ft_isalnum(key[i]) == 1 || key[i] == '_'))
// 		i++;
// 	// printf("key_len: %d\n", i);
// 	return (i);
// }

// int	ft_value_len(t_minishell *mini, char *key)
// {
// 	int		i;
// 	int		keylen;
// 	char	*k_key;
// 	char	*value;

// 	keylen = ft_key_len(mini, key);
// 	k_key = (char *)ft_calloc(1, sizeof(char) * keylen + 1);
// 	if (k_key == NULL)
// 		return (-1);
// 	i = 0;
// 	while (i < keylen)
// 	{
// 		k_key[i] = key[i];
// 		i++;
// 	}
// 	k_key[i] = '\0';
// 	// printf("%s\n", k_key);
// 	value = ft_get_env_var(mini, k_key); //check
// 	if (value == NULL)
// 		return (-1);
// 	i = 0;
// 	i = ft_strlen(value);
// 	free (value);
// 	free (k_key);
// 	return (i);
// }

// int	ft_get_expand_len(t_minishell *mini, char *str, t_cmd *head)
// {
// 	int		i;
// 	int		len;
// 	(void)head;

// 	i = 0;
// 	len = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\'')
// 		{
// 			i++;
// 			while (str[i] != '\'' && str[i] != '\0')
// 			{
// 				i++;
// 				len++;
// 			}
// 			if (str[i] == '\0')
// 			{
// 				return (-1);
// 			}
// 			i++;
// 		}
// 		else if (str[i] == '\"')
// 		{
// 			i++;
// 			while (str[i] != '\"' && str[i] != '\0')
// 			{
// 				if (str[i] == '$')
// 				{
// 					i++;
// 					i = i + ft_key_len(mini, &str[i]);
// 					len = len + ft_value_len(mini, &str[i]);
// 				}
// 				else
// 				{
// 					i++;
// 					len++;
// 				}
// 			}
// 			if (str[i] == '\0')
// 			{
// 				return (-1);
// 			}
// 		}
// 		else if (str[i] == '$')
// 		{
// 			i++;
// 			i = i + ft_key_len(mini, &str[i]);
// 			len = len + ft_value_len(mini, &str[i]);
// 		}
// 		else
// 		{
// 			i++;
// 			len++;
// 		}
// 	}
// 	return (len);
// }

// char	*ft_get_value_str(t_minishell *mini, char *key, int keylen)
// {
// 	int		i;
// 	char	*k_key;
// 	char	*value;

// 	k_key = (char *)malloc(sizeof(char) * keylen + 1);
// 	if (k_key == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (i < keylen)
// 	{
// 		k_key[i] = key[i];
// 		i++;
// 	}
// 	// printf("keylen: %d, key: %s, k_key: %s\n", keylen, key, k_key);
// 	k_key[i] = '\0';
// 	// printf("d1: %s\n", k_key);
// 	value = ft_get_env_var(mini, k_key);
// 	free (k_key);
// 	return (value);
// }

// char	*ft_get_expand_str(t_minishell *mini, char *str, char *final, t_cmd *head)
// {
// 	int		i;
// 	int		j;
// 	int		len;
// 	char	*temp;
// 	(void)head;

// 	i = 0;
// 	len = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\'')
// 		{
// 			i++;
// 			while (str[i] != '\'' && str[i] != '\0')
// 			{
// 				final[len] = str[i];
// 				i++;
// 				j++;
// 			}
// 			if (str[i] == '\0')
// 			{
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		else if (str[i] == '\"')
// 		{
// 			i++;
// 			while (str[i] != '\"' && str[i] != '\0')
// 			{
// 				if (str[i] == '$')
// 				{
// 					i++;
// 					j = ft_key_len(mini, &str[i]);
// 					temp = ft_get_value_str(mini, &str[i], j);
// 					i = i + j;
// 					j = 0;
// 					while (temp[j] != '\0')
// 					{
// 						final[len] = temp[j];
// 						len++;
// 						j++;
// 					}
// 					free (temp);
// 					temp = NULL;
// 				}
// 				else
// 				{
// 					final[len] = str[i];
// 					i++;
// 					len++;
// 				}
// 			}
// 			if (str[i] == '\0')
// 			{
// 				return (NULL);
// 			}
// 		}
// 		else if (str[i] == '$')
// 		{
// 			i++;
// 			j = ft_key_len(mini, &str[i]);
// 			temp = ft_get_value_str(mini, &str[i], j);
// 			i = i + j;
// 			j = 0;
// 			while (temp[j] != '\0')
// 			{
// 				final[len] = temp[j];
// 				len++;
// 				j++;
// 			}
// 			free (temp);
// 			temp = NULL;
// 		}
// 		else
// 		{
// 			final[len] = str[i];
// 			i++;
// 			len++;
// 		}
// 	}
// 	return (final);
// }

// char	*ft_expand_str(t_minishell *mini, char *str, t_cmd *head)
// {
// 	// int		i;
// 	// int		j;
// 	int		len;
// 	char	*final;

// 	len = ft_get_expand_len(mini, str, head);
// 	final = (char *)malloc(sizeof(char) * len + 1);
// 	if (final == NULL)
// 		return (NULL);
// 	final = ft_get_expand_str(mini, str, final, head);
// 	// printf ("ft_expand_str, final: %s\n", final);
// 	if (final == NULL)
// 		return (NULL);
// 	return (final);
// }

// int	ft_expand_all(t_minishell *mini)
// {
// 	t_cmd	*head;
// 	int		i;
// 	int		j;
// 	char	*final;

// 	j = 0;
// 	head = mini->cmd;
// 	while (head != NULL)
// 	{
// 		i = 0;
// 		while (head->args[i] != NULL)
// 		{
// 			if (head->quote_type[i] != 1)
// 			{
// 				final = ft_expand_str(mini, head->args[i], head);
// 				free (head->args[i]);
// 				head->args[i] = final;
// 			}
// 			i++;
// 		}
// 		head = head->next;
// 	}
// 	return (0);
// }

// int	ft_malloc_error(t_minishell *mini)
// {
// 	mini->last_exit = ENOMEM;
// 	ft_error_msg("malloc", NULL, strerror(ENOMEM));
// 	return (1);
// }

// void	ft_free_ex(char **s1, char *s2)
// {
// 	int		i;

// 	i = 0;
// 	if (s1 != NULL)
// 	{
// 		while (s1[i] != NULL)
// 		{
// 			free (s1[i]);
// 			s1[i] = NULL;
// 			i++;
// 		}
// 		free (s1);
// 		s1 = NULL;
// 	}
// 	if (s2 != NULL)
// 	{
// 		free (s2);
// 		s2 = NULL;
// 	}
// }

// //will return the value if the key is set, if the key is unset, will return "". In case of critical failure, will return NULL
// char	*ft_get_env_var(t_minishell *mini, char *key)
// {
// 	int		i;
// 	int		j;
// 	char	*temp;
// 	char	*final;
// 	int		len;

// 	i = 0;
// 	temp = ft_strjoin(key, "=");
// 	if (temp == NULL)
// 		return (mini->last_exit = errno, perror("malloc"), NULL);
// 	len = ft_strlen(temp);
// 	while (mini->env[i] != NULL)
// 	{
// 		if (ft_strncmp(mini->env[i], temp, len) == 0)
// 		{
// 			free (temp);
// 			j = ft_strlen(mini->env[i]);
// 			final = ft_substr(mini->env[i], len, j - len);
// 			if (final == NULL)
// 				return (mini->last_exit = errno, perror("malloc"), NULL);
// 			return (final);
// 		}
// 		i++;
// 	}
// 	free (temp);
// 	final = ft_strdup("");
// 	if (final == NULL)
// 		return (mini->last_exit = errno, perror("malloc"), NULL);
// 	return (final);
// }

// char	**ft_realloc_env(char **old_env)
// {
// 	char	**new_env;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	while (old_env[i] != NULL)
// 		i++;
// 	new_env = (char **)malloc(sizeof(char *) * (i + 2));
// 	if (new_env == NULL)
// 		return (NULL);
// 	j = 0;
// 	while (j < i)
// 	{
// 		new_env[j] = old_env[j];
// 		j++;
// 	}
// 	new_env[j] = NULL;
// 	new_env[++j] = NULL;
// 	free(old_env);
// 	return (new_env);
// }

// int	ft_set_env_var(t_minishell *mini, char *key, char *ans) //will return 0 on success and will return 1 in case of critical malloc failure
// {
// 	int		i;
// 	char	*temp;
// 	char	*temp2;
// 	int		len;

// 	temp = ft_strjoin(key, "=");
// 	if (temp == NULL)
// 		return (ft_malloc_error(mini));
// 	temp2 = ft_strjoin(temp, ans);
// 	if (temp2 == NULL)
// 		return (ft_malloc_error(mini));
// 	i = 0;
// 	len = ft_strlen(temp);
// 	while (mini->env[i] != NULL)
// 	{
// 		if (ft_strncmp(mini->env[i], temp, len) == 0)
// 			break ;
// 		i++;
// 	}
// 	printf("set_env_debug\ntemp: %s\ntemp2: %s\n, mini->env: %s\n", temp, temp2, mini->env[i]);
// 	if (mini->env[i] != NULL)
// 	{
// 		free (mini->env[i]);
// 		mini->env[i] = NULL;
// 		mini->env[i] = temp2;
// 		printf("check in set func: %s\n", mini->env[i]);
// 		return (0);
// 	}
// 	mini->env = ft_realloc_env(mini->env);
// 	if (mini->env == NULL)
// 		return (free (temp2), ft_malloc_error(mini));
// 	mini->env[i] = temp2;
// 	mini->env[i + 1] = NULL;
// 	free (temp);
// 	return (0);
// }

// char	*ft_join_strs(char *s1, char *s2, char *s3, char *s4)
// {
// 	int		i;
// 	int		j;
// 	char	*str;
// //ENSURE FT_STRLEN DOES A NULL CHECK. ELSE SEG FAULT WILL OCCUR!!!!
// 	j = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + ft_strlen(s4);
// 	str = (char *)malloc(sizeof(char) * (j + 1));
// 	if (str == NULL)
// 		return (NULL);
// 	i = -1;
// 	j = 0;
// 	while (s1 && s1[++i] != '\0')
// 		str[j++] = s1[i];
// 	i = -1;
// 	while (s2 && s2[++i] != '\0')
// 		str[j++] = s2[i];
// 	i = -1;
// 	while (s3 && s3[++i] != '\0')
// 		str[j++] = s3[i];
// 	i = -1;
// 	while (s4 && s4[++i] != '\0')
// 		str[j++] = s4[i];
// 	str[j] = '\0';
// 	return (str);
// }

// void	ft_error_msg(char *command, char *key, char *error_msg)
// {
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(command, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	if (key != NULL)
// 	{
// 		if (ft_strncmp(command, "export", 7) == 0 || ft_strncmp(command, "unset", 6) == 0)
// 			ft_putstr_fd("'", STDERR_FILENO);
// 		ft_putstr_fd(key, STDERR_FILENO);
// 		if (ft_strncmp(command, "export", 7) == 0 || ft_strncmp(command, "unset", 6) == 0)
// 			ft_putstr_fd("'", STDERR_FILENO);
// 		ft_putstr_fd(": ", STDERR_FILENO);
// 	}
// 	ft_putendl_fd(error_msg, STDERR_FILENO);
// 	return ;
// }

// /*End of ft_env_utils.c*/


// /*Start of ft_system_cmd.c*/

// char	*get_final_path(t_minishell *mini, char **all_path, char *path)
// {
// 	char	*full_path;
// 	int		i;

// 	i = 0;
// 	while (all_path[i] != NULL)
// 	{
// 		full_path = ft_strjoin(all_path[i], path);
// 		if (full_path == NULL)
// 			return (ft_free_ex(all_path, path), mini->last_exit = errno, perror("malloc"), NULL);
// 		if (access(full_path, F_OK | X_OK) == 0)
// 			return (ft_free_ex(all_path, path), full_path);
// 		i++;
// 	}
// 	mini->last_exit = 127; //cmd not found
// 	return (NULL);
// }

// char	*get_cmd_path(t_minishell *mini, char const *to_find)
// {
// 	char	*path; // will store "/command" and test against all path if the path is valid and is accessible
// 	char	*temp; //temp variable that stores all paths
// 	char	**all_path; //to split temp and get all paths in a 2D array

// 	temp = ft_get_env_var(mini, "PATH");
// 	if (temp == NULL) //if path is unset from the original shell env, exit.
// 	{
// 		ft_putendl_fd("Error: PATH not found", STDERR_FILENO);
// 		mini->last_exit = errno;
// 		return (NULL);
// 	}
// 	all_path = ft_split(temp, ':');
// 	if (all_path == NULL)
// 		return (mini->last_exit = errno, perror("malloc"), NULL);
// 	free (temp);
// 	temp = NULL;
// 	path = ft_strjoin("/", to_find);
// 	if (path == NULL)
// 		return (ft_free_ex(all_path, NULL), mini->last_exit = errno, perror("malloc"), NULL);
// 	return (get_final_path(mini, all_path, path));
// }

// int	ft_system_cmd(t_minishell *mini, t_cmd *current_cmd)
// {
// 	char	*path;

// 	path = get_cmd_path(mini, current_cmd->args[0]);
// 	// printf("path: %s\n", path);
// 	if (path == NULL)
// 	{
// 		if (mini->last_exit == 127)
// 		{
// 			// printf("debug system_cmd cmd not found check\n");
// 			ft_error_msg(current_cmd->args[0], NULL, strerror(127));
// 			return (0);
// 		}
// 		else
// 			return (1);
// 	}
// 	// printf("ft_system_cmd\npath: %s, args[0]: %s, args[1]: %s", path, current_cmd->args[0], current_cmd->args[1]);
// 	if (execve(path, current_cmd->args, mini->env) == -1)
// 	{
// 		// printf("check execve\n");
// 		mini->last_exit = errno;
// 		free (path);
// 		path = NULL;
// 		ft_error_msg(current_cmd->args[0], NULL, strerror(errno));
// 	}
// 	// printf("debug system_cmd\n");
// 	free (path);
// 	path = NULL;
// 	return (0);
// 	//write the code to execute and check on the error message calls.
// }
// /*End of ft_system_cmd.c*/

// /*Start of all inbuilt cmds which will be saved as ft_cmd_name.c*/
// /*returns 1 in case of critical emergency. 0 otherwise*/
// int	ft_cd(t_minishell *mini, t_cmd *current_cmd)
// {
// 	char	*path;
// 	char	*temp;
// 	char	pwd[PATH_MAX]; 

// 	if (current_cmd->args[1] != NULL && current_cmd->args[2] != NULL)
// 	{
// 		mini->last_exit = 1; //errno for too many arguments
// 		ft_error_msg(current_cmd->args[0], NULL, "too many arguments");
// 		return (0);
// 	}
// 	if (current_cmd->args[1] == NULL || current_cmd->args[1][0] == '\0' || (ft_strncmp(current_cmd->args[1], "~", 2) == 0))
// 		path = ft_get_env_var(mini, "HOME");
// 	else if (ft_strncmp(current_cmd->args[1], "-", 2) == 0)
// 		path = ft_get_env_var(mini, "OLDPWD");
// 	else
// 		path = ft_strdup(current_cmd->args[1]);
// 	if (path == NULL)
// 		return (ft_malloc_error(mini));
// 			printf("CD debug: raw path = '%s'\n", path);
// 	for (int k = 0; k < (int)strlen(path); k++)
//     printf("path[%d] = %d\n", k, (unsigned char)path[k]);

// 	printf ("pwd =%s\n", ft_get_env_var(mini, "PWD")); //check for cd ~
// 	printf ("oldpwd =%s\n", ft_get_env_var(mini, "OLDPWD"));
// 	printf("check 2: temp: %s\n", ft_get_env_var(mini, "PWD"));
// 	if (chdir(path) != 0)
// 	{
// 		free (path);
// 		mini->last_exit = errno;
// 		ft_error_msg(current_cmd->args[0], current_cmd->args[1], strerror(errno));
// 		return (0);
// 	}
// 	free (path);
// 	temp = ft_get_env_var(mini, "PWD");
// 	if (temp == NULL)
// 		return (ft_malloc_error(mini));
// 	if (ft_set_env_var(mini, "OLDPWD", temp) != 0)
// 		return (free (temp), ft_malloc_error(mini));
// 	free (temp);
// 	temp = NULL;
// 	if (getcwd(pwd, PATH_MAX) == NULL)
// 	{
// 		mini->last_exit = errno;
// 		ft_error_msg(current_cmd->args[0], current_cmd->args[1], strerror(errno));
// 		return (1);
// 	}
// 	temp = ft_strdup(pwd);
// 	// printf("temp: %s\n pwd: %s\n", temp, pwd);
// 	if (temp == NULL)
// 		return (ft_malloc_error(mini));
// 	if (ft_set_env_var(mini, "PWD", temp) != 0)
// 		return (free (temp), ft_malloc_error(mini));
		
// 	free (temp);
// 	mini->last_exit = 0;
// 	return (0);
// }

// int	ft_echo(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		argc;
// 	int		i;
// 	int		new_line;

// 	argc = 0;
// 	new_line = 0;
// 	while (current_cmd->args[argc] != NULL)
// 		argc++;
// 	if (argc > 1 && (ft_strncmp(current_cmd->args[1], "-n", 3) == 0))
// 		new_line = 1;
// 	i = 1;
// 	while ((i + new_line) < argc)
// 	{
// 		printf("%s", current_cmd->args[i + new_line]);
// 		i++;
// 		if ((i + new_line) < argc)
// 			printf(" ");
// 	}
// 	if (new_line != 1)
// 		printf("\n");
// 	mini->last_exit = 0;
// 	return (0);
// }

// int	ft_env(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		i;

// 	i = 0;
// 	while (current_cmd->args[i] != NULL)
// 		i++;
// 	if (i > 1)
// 	{
// 		mini->last_exit = 1; //reconfirm the errno
// 		ft_error_msg(current_cmd->args[0], NULL, "too many arguments");
// 		return (0);
// 	}
// 	i = 0;
// 	while (mini->env[i] != NULL)
// 	{
// 		ft_putendl_fd(mini->env[i], STDOUT_FILENO);
// 		i++;
// 	}
// 	mini->last_exit = 0;
// 	return (0);
// }

// int	ft_pwd(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		i;
// 	char	*pwd;

// 	i = 0;
// 	while (current_cmd->args[i] != NULL)
// 		i++;
// 	if (i > 1)
// 	{
// 		mini->last_exit = 1; //reconfirm the errno
// 		ft_error_msg(current_cmd->args[0], NULL, "too many arguments");
// 		return (0);
// 	}
// 	pwd = ft_get_env_var(mini, "PWD");
// 	if (pwd == NULL)
// 		return (ft_malloc_error(mini));
// 	ft_putendl_fd(pwd, STDIN_FILENO);
// 	free (pwd);
// 	mini->last_exit = 0;
// 	return (0);
// }

// //check for valid env name.
// int	ft_check_env_name(char *name)
// {
// 	int		i;

// 	i = 0;
// 	if (ft_isalpha(name[0]) == 1 || name[0] == '_')
// 		i++;
// 	else
// 		return (1);
// 	while (name[i] != '\0' || name[i] != '=')
// 	{
// 		if (ft_isalnum(name[i]) == 1 || name[i] == '_')
// 			i++;
// 		else
// 			return (1);
// 	}
// 	return (0);
// }

// int	ft_get_value_len(char *str)
// {
// 	int		i;
// 	int		len;
// 	int		value_len;

// 	i = 0;
// 	len = ft_strlen(str);
// 	value_len = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\'')
// 		{
// 			i++;
// 			while (str[i] != '\'')
// 			{
// 				if (str[i] == '\0')
// 					return (-1);
// 				value_len++;
// 				i++;
// 			}
// 			i++;
// 		}
// 		else if (str[i] == '\"')
// 		{
// 			i++;
// 			while (str[i] != '\"')
// 			{
// 				if (str[i] == '\0')
// 					return (-1);
// 				value_len++;
// 				i++;
// 			}
// 			i++;
// 		}
// 		else
// 		{
// 			value_len++;
// 			i++;
// 		}
// 	}
// 	return (value_len);
// }

// char	*ft_remove_quote_exec(char *str, int len)
// {
// 	int		i;
// 	char	*value;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	value = (char *)malloc(sizeof(char) * (len + 1));
// 	if (value == NULL)
// 		return (NULL);
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\'')
// 		{
// 			i++;
// 			while (str[i] != '\'')
// 			{
// 				value[j] = str[i];
// 				j++;
// 				i++;
// 			}
// 			i++;
// 		}
// 		else if (str[i] == '\"')
// 		{
// 			i++;
// 			while (str[i] != '\"')
// 			{
// 				if (str[i] == '$')
// 					//call expand variable
// 				value[j] = str[i];
// 				j++;
// 				i++;
// 			}
// 			i++;
// 		}
// 		else
// 		{
// 			if (str[i] == '$')
// 				//call expand variable
// 			value[j] = str[i];
// 			j++;
// 			i++;
// 		}
// 	}
// 	value[j] = '\0';
// 	return (value);
// }

// int	ft_export_util(t_minishell *mini, char *full_str)
// {
// 	int		i;
// 	int		value_len;
// 	// char	*new_full;
// 	char	*key;
// 	char	*value;

// 	i = 0;
// 	while (full_str[i] != '\0' || full_str[i] != '=')
// 		i++;
// 	if (i == (int)ft_strlen(full_str))
// 		return (0);
// 	key = ft_substr(full_str, 0, i);
// 	if (key == NULL)
// 		return (1);
// 	i++;
// 	value_len = ft_get_value_len(&full_str[i]);
// 	if (value_len == -1)
// 		return (mini->last_exit = 1, free (key), ft_error_msg("export", NULL, "syntax error: unexpected end of file"), 1);
// 	value = ft_remove_quote_exec(&full_str[i], value_len);
// 	if (value == NULL)
// 		return (free (key), 1);
// 	if (ft_set_env_var(mini, key, value) != 0)
// 		i = 1;
// 	else
// 		i = 0;
// 	free (key);
// 	free (value);
// 	// free (new_full);
// 	return (i);
// }

// int	ft_export(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		i;
// 	int		export_error;
// 	// int		del_no_of_quotes;

// 	i = 1;
// 	export_error = 0;
// 	while (current_cmd->args[i] != NULL)
// 	{
// 		if (ft_check_env_name(current_cmd->args[i]) == 1)
// 		{
// 			export_error = 1;
// 			ft_error_msg("export", current_cmd->args[i], "not a valid identifier");
// 		}
// 		else
// 		{
// 			if (ft_export_util(mini, current_cmd->args[i]) == 1)
// 				return (ft_malloc_error(mini));
// 		}
// 		i++;
// 	}
// 	mini->last_exit = export_error;
// 	return (0);
// }

// int	ft_check_unset_name(char *name)
// {
// 	int		i;

// 	if (ft_isalpha(name[0]) != 1 || name[0] != '_')
// 		return (1);
// 	i = 1;
// 	while (name[i] != '\0')
// 	{
// 		if (ft_isalnum(name[i]) != 1 || name[i] != '_')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// int	ft_unset_util(t_minishell *mini, char *key)
// {
// 	int		i;
// 	int		len;
// 	char	*temp;

// 	i = 0;
// 	temp = ft_strjoin(key, "=");
// 	if (temp == NULL)
// 		return (ft_malloc_error(mini));
// 	len = ft_strlen(temp);
// 	while (mini->env[i] != NULL)
// 	{
// 		if (ft_strncmp(mini->env[i], temp, len) == 0)
// 			break ;
// 		i++;
// 	}
// 	if (mini->env[i] == NULL)
// 		return (0);
// 	free (mini->env[i]);
// 	mini->env[i] = NULL;
// 	i++;
// 	while (mini->env[i] != NULL)
// 	{
// 		mini->env[i] = mini->env[i + 1]; //can realloc, but this should work as well.
// 		i++;
// 	}
// 	return (0);
// }

// int	ft_unset(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		i;
// 	int		unset_error;

// 	i = 1;
// 	unset_error = 0;
// 	while (current_cmd->args[i] != NULL)
// 	{
// 		if (ft_check_unset_name(current_cmd->args[i]) == 1)
// 		{
// 			unset_error = 1;
// 			ft_error_msg("unset", current_cmd->args[i], "not a valid identifier");
// 		}
// 		else if (ft_unset_util(mini, current_cmd->args[i]) == 1)
// 				return (ft_malloc_error(mini));
// 		i++;
// 	}
// 	mini->last_exit = unset_error;
// 	return (0);
// }

// int	ft_exit(t_minishell *mini, t_cmd *current_cmd)
// {
// 	int		i;

// 	if (current_cmd != NULL)
// 		i = 0;
// 	ft_terminate_execute(mini);
// 	printf("exit\n");
// 	exit (0);
// }

// /*End of all inbuilt cmds*/

// /*Start of initiate_execution.c*/

// //will return 1 on critical failure. Will return 0 otherwise.
// int	begin_exec_cmd(t_minishell *mini, t_cmd *current_cmd)
// {
// 	if (ft_strncmp(current_cmd->args[0], "cd", 3) == 0)
// 		return (ft_cd(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "echo", 5) == 0)
// 		return (ft_echo(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "env", 4) == 0)
// 		return (ft_env(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "export", 7) == 0)
// 		return (ft_export(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "pwd", 4) == 0)
// 		return (ft_pwd(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "unset", 6) == 0)
// 		return (ft_unset(mini, current_cmd));
// 	else if (ft_strncmp(current_cmd->args[0], "exit", 5) == 0)
// 		return (ft_exit(mini, current_cmd));
// 	else
// 		return (ft_system_cmd(mini, current_cmd));
// }

// // void	initiate_execute(t_minishell *mini, int *fd, int count_cmd) //fn length too long, to shorten it
// // {
// // 	int		i;
// // 	int		j;
// // 	t_cmd	*head;
// // 	pid_t	pid;

// // 	head = mini->cmd;
// // 	i = 0;
// // 	j = 0;
// // 	while (head != NULL)
// // 	{
// // 		//check on fork. I am forking one extra
// // 		if (head->next != NULL)
// // 			pid = fork();
// // 		if (head->next != NULL && pid == -1)
// // 		{
// // 			perror("fork");
// // 			return ;
// // 		}
// // 		if (head->next == NULL || pid == 0)
// // 		{
// // 			if (head->next != NULL) //check if there is a next cmd
// // 				dup2(fd[i + 1], head->outfile);
// // 			if (i != 0) //check if there is a prev cmd
// // 				dup2(fd[i - 2], head->infile); //need to check if this works with the same infile or the prev infile
// // 			j = -1;
// // 			while (++j < 2 * (count_cmd - 1))
// // 				close(fd[j]);
// // 			if (begin_exec_cmd(mini, head) == 1)
// // 			{
// // 				printf("debug initiate_execute\n"); //in case of a critical error exit
// // 				return ; //to modify depending on what to close and to wait for child processes
// // 			}
// // 		}
// // 		i = i + 2;
// // 		head = head->next;
// // 	}
// // 	j = -1;
// // 	// closing all parent fds
// // 	while (++j < (2 * (count_cmd - 1)))
// // 		close(fd[j]);
// // 	j = -1;
// // 	// waiting for child process to end
// // 	while (++j < count_cmd)
// // 		wait(NULL);
// // }


// void	initiate_execute(t_minishell *mini, int *fd, int count_cmd)
// {
// 	int		i;
// 	int		j;
// 	t_cmd	*head;
// 	pid_t	pid;

// 	head = mini->cmd;
// 	i = 0;
// 	j = 0;
// 	/* Iterate through each cmd in the list. */
// 	while (head != NULL)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			return ;
// 		}
// 		if (pid == 0) /* Child procces */
// 		{
// 			/* === INPUT HANDLING === / /

// 			#If the command has an infile different from STDIN_FILENO,
// 			#it means the user has specified input redirection using < or <<.
// 			#In this case, replace the standard input with infile.
// 			#Otherwise, if this is not the first command (i != 0), 
// 			#read from the pipe. */
// 			if (head->infile != STDIN_FILENO)
// 			{
// 				if (dup2(head->infile, STDIN_FILENO) == -1)
// 				{
// 					perror("dup2 (file-in)");
// 					exit(1);
// 				}
// 				close(head->infile); 
// 			}
// 			else if (i != 0) 
// 			{
// 				if (dup2(fd[i - 2], STDIN_FILENO) == -1)
// 				{
// 					perror("dup2 (pipe-in)");
// 					exit(1);
// 				}
// 			}
// 			/* === OUTPUT HANDLING === / /
// 			#Similarly for output: if outfile != STDOUT_FILENO,
// 			#it means the user has specified redirection with > or >>;
// 			#otherwise, if there is a next command (head->next != NULL),
// 			#redirect the output to a pipe. */
// 			if (head->outfile != STDOUT_FILENO)
// 			{
// 				if (dup2(head->outfile, STDOUT_FILENO) == -1)
// 				{
// 					perror("dup2 (file-out)");
// 					exit(1);
// 				}
// 				close(head->outfile);
// 			}
// 			else if (head->next != NULL)
// 			{
// 				if (dup2(fd[i + 1], STDOUT_FILENO) == -1)
// 				{
// 					perror("dup2 (pipe-out)");
// 					exit(1);
// 				}
// 			}

// 			/* Close all pipes, as they are no longer needed in the child process. */
// 			j = 0;
// 			while (j < 2 * (count_cmd))
// 			{
// 				close(fd[j]);
// 				j++;
// 			}

// 			/* Directly execute the command (or built-in). */
// 			if (begin_exec_cmd(mini, head) == 1)
// 				// exit(1);

// 			exit(mini->last_exit);
// 		}
// 		i = i + 2;
// 		head = head->next;
// 	}
// 	/*The parent process closes all pipes.*/
// 	j = -1;
// 	while (++j < (2 * (count_cmd)))
// 		close(fd[j]);
// 	/* And waits for all child processes to finish. */
// 	j = -1;
// 	while (++j < count_cmd)
// 		wait(NULL);
// }



// // void	initiate_execute(t_minishell *mini, int *fd, int count_cmd) //fn length too long, to shorten it
// // {
// // 	int		i;
// // 	int		j;
// // 	t_cmd	*head;
// // 	pid_t	pid;

// // 	head = mini->cmd;
// // 	i = 0;
// // 	j = 0;
// // 	while (head != NULL)
// // 	{
// // 		//check on fork. I am forking one extra
// // 		pid = fork();
// // 		if (pid == -1)
// // 		{
// // 			perror("fork");
// // 			return ;
// // 		}
// // 		if (pid == 0)
// // 		{
// // 			if (head->next != NULL) //check if there is a next cmd
// // 				dup2(fd[i + 1], head->outfile);
// // 			if (i != 0) //check if there is a prev cmd
// // 				dup2(fd[i - 2], head->infile); //need to check if this works with the same infile or the prev infile
// // 			j = -1;
// // 			while (++j < 2 * (count_cmd))
// // 				close(fd[j]);
// // 			if (begin_exec_cmd(mini, head) == 1)
// // 			{
// // 				// printf("debug initiate_execute\n"); //in case of a critical error exit
// // 				return ; //to modify depending on what to close and to wait for child processes
// // 			}
// // 		}
// // 		i = i + 2;
// // 		head = head->next;
// // 	}
// // 	j = -1;
// // 	// closing all parent fds
// // 	while (++j < (2 * (count_cmd)))
// // 		close(fd[j]);
// // 	j = -1;
// // 	// waiting for child process to end
// // 	while (++j < count_cmd)
// // 		wait(NULL);
// // }

// /*End of initiate_execute.c*/

// /*start of expand_variables.c*/
// //to write detailed code later

// int	expand_variables(t_minishell *mini)
// {
// 	if (mini != NULL)
// 		return (0);
// 	return (1);
// }
// /*end of expand_variables.c*/

// /*start of execute.c*/

// // int	*create_pipes(t_minishell *mini, int count_cmd)
// // {
// // 	int		*fd;
// // 	int		i;
// // 	t_cmd	*head;

// // 	i = 0;
// // 	head = mini->cmd;
// // 	fd = ft_calloc(2 * (count_cmd - 1), sizeof(int));
// // 	if (count_cmd > 1 && fd == NULL)
// // 	{
// // 		mini->last_exit = errno;
// // 		return (perror("malloc"), NULL);
// // 	}
// // 	while (i < count_cmd - 1)
// // 	{
// // 		if (pipe(fd + (i * 2)) < 0)
// // 		{
// // 			mini->last_exit = errno;
// // 			return (perror("pipe"), NULL);
// // 		}
// // 		i++;
// // 	}
// // 	return (fd);
// // }

// int	*create_pipes(t_minishell *mini, int count_cmd)
// {
// 	int		*fd;
// 	int		i;
// 	t_cmd	*head;

// 	i = 0;
// 	head = mini->cmd;
// 	fd = ft_calloc(2 * (count_cmd), sizeof(int));
// 	if (fd == NULL)
// 	{
// 		mini->last_exit = errno;
// 		return (perror("malloc"), NULL);
// 	}
// 	while (i < count_cmd)
// 	{
// 		if (pipe(fd + (i * 2)) < 0)
// 		{
// 			mini->last_exit = errno;
// 			return (perror("pipe"), NULL);
// 		}
// 		i++;
// 	}
// 	return (fd);
// }

// int	handle_inout_fd(t_cmd *head)
// {
// 	int		count_cmd;

// 	count_cmd = 0;
// 	while (head != NULL)
// 	{
// 		count_cmd++;
// 		if (head->infile < 0)
// 			head->infile = STDIN_FILENO;
// 		if (head->outfile < 0)
// 			head->outfile = STDOUT_FILENO;
// 		head = head->next;
// 	}
// 	return (count_cmd);
// }

// void	ft_terminate_execute(t_minishell *mini)
// {
// 	t_cmd	*head;
// 	t_cmd	*next;
// 	int		i;

// 	head = mini->cmd;
// 	while (head != NULL)
// 	{
// 		next = head->next;
// 		i = -1;
// 		while (head->args && head->args[++i] != NULL)
// 		{
// 			free (head->args[i]);
// 			head->args = NULL;
// 		}
// 		free (head->args);
// 		head->args = NULL;
// 		free (head);
// 		head = NULL;
// 		head = next;
// 	}
// 	// if (mini->cmd != NULL)
// 	// {
// 	// 	printf("check 7\n");
// 	// 	free (mini->cmd);
// 	// 	mini->cmd = NULL;
// 	// }
// 	free (mini);
// }

// // void	ft_debug(t_minishell *mini)
// // {
// // 	t_cmd	*head;
// // 	int		i;
// // 	int		j;

// // 	i = 0;
// // 	j = 0;
// // 	head = mini->cmd;
// // 	// printf("ft_debug\n");
// // 	while (head != NULL)
// // 	{
// // 		i = 0;
// // 		printf("cmd: %d\n", ++j);
// // 		while (head->args[i] != NULL)
// // 		{
// // 			printf ("args[%d] = %s; quote_type: %d\n", i, head->args[i], head->quote_type[i]);
// // 			i++;
// // 		}
// // 		printf ("\n");
// // 		head = head->next;
// // 	}
// // }

// void	execute(t_minishell *mini)
// {
// 	int		*fd;
// 	int		count_cmd;

// 	// ft_debug(mini);
// 	count_cmd = handle_inout_fd(mini->cmd);
// 	// ft_expand_all(mini);
// 	fd = NULL;
// 	fd = create_pipes(mini, count_cmd);
// 	if ((fd != NULL)) //to write expand_variables later
// 	{
// 		// printf ("debug pre initiate\n");
// 		initiate_execute(mini, fd, count_cmd);
// 		// printf("debug execute\n");
// 	}
// 	// ft_terminate_execute(mini);
// }