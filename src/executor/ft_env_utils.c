/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:16 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 10:03:57 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
