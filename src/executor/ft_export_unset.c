/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:49:47 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 11:49:13 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_export_util_2(t_minishell *mini)
{
	int		i;
	char	**env;
	int		j;

	env = mini->env;
	i = -1;
	while (env[++i] != NULL)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		j = 0;
		while (env[i][j] != '\0' && env[i][j] != '=')
		{
			write(STDOUT_FILENO, &env[i][j], 1);
			j++;
		}
		if (env[i][j] == '\0')
			printf("=\"\"\n");
		else
		{
			printf("=\"");
			printf("%s\"\n", &env[i][j + 1]);
		}
	}
	return (0);
}

int	ft_export(t_minishell *mini, t_cmd *head)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	if (head->args[1] == NULL || head->args[1][0] == '\0')
		return (ft_export_util_2(mini));
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
	return (exit_status);
}

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
