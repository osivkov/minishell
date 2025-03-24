/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_echo_env_pwd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:12 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 11:27:35 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_cd(t_minishell *mini, t_cmd *head)
{
	char	*path;
	char	pwd[PATH_MAX];

	if (head->args[1] != NULL && head->args[2] != NULL)
	{
		ft_error_msg(head->args[0], NULL, "too many arguments");
		return (1);
	}
	if (head->args[1] == NULL || head->args[1][0] == '\0' || \
		ft_strncmp(head->args[1], "~", 2) == 0)
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

/*new_line and j must be set to 0*/
int	ft_echo(t_minishell *mini, t_cmd *head, int new_line, int j)
{
	int		i;

	while (head->args[++j] != NULL && ft_strncmp((head->args[j]), "-n", 2) == 0)
	{
		i = 2;
		while (head->args[j][i] != '\0' && head->args[j][i] == 'n')
			i++;
		if (head->args[j][i] != '\0')
			break ;
		new_line++;
	}
	i = 1 + new_line;
	while (head->args[i] != NULL)
	{
		printf("%s", head->args[i]);
		i++;
		if (head->args[i] != NULL)
			printf(" ");
	}
	if (new_line == 0)
		printf("\n");
	mini->last_exit = 0;
	return (0);
}

//new_line and d_flag is set to 0
// int	ft_echo(t_minishell *mini, t_cmd *head, int new_line, int d_flag)
// {
// 	int		i;

// 	if (head->args[1] != NULL && ft_strncmp((head->args[1]), "-n", 2) == 0)
// 	{
// 		i = 2;
// 		while (head->args[1][i] != '\0' && head->args[1][i] == 'n')
// 			i++;
// 		if (i == 2 && head->args[1][i] == '\0')
// 			new_line = 1;
// 		else if (i > 2 && head->args[1][i] == '\0')
// 			d_flag = 1;
// 	}
// 	i = 1 + d_flag + new_line;
// 	while (head->args[i] != NULL)
// 	{
// 		printf("%s", head->args[i]);
// 		i++;
// 		if (head->args[i] != NULL)
// 			printf(" ");
// 	}
// 	if (new_line != 1 && d_flag != 1)
// 		printf("\n");
// 	mini->last_exit = 0;
// 	return (0);
// }

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
