
// #include "minishell.h"

// char	*ft_get_env_var(t_minishell *mini, char *key);
// char	*ft_join_strs(char *s1, char *s2, char *s3, char *s4);

// int	ft_key_len(t_minishell *mini, char *key)
// {
// 	int		i;

// 	i = 0;
// 	if (ft_isalpha(key[i]) != 0 && key[i] != '_')
// 		return (0);
// 	while (ft_isalnum(key[i]) == 0 || key[i] == '_')
// 		i++;
// 	return (i);
// }

// int	ft_value_len(t_minishell *mini, char *key)
// {
// 	char	*final_key;
// 	char	*final_value;
// 	int		var_len;

// 	final_key = ft_substr(key, 0, ft_key_len(mini, key));
// 	if (final_key == NULL)
// 		return (-1);
// 	final_value = ft_get_env_var(mini, final_key);
// 	if (final_value == NULL)
// 	{
// 		free (final_key);
// 		final_key = NULL;
// 		return (-1);
// 	}
// 	var_len = ft_strlen(final_value);
// 	free (final_key);
// 	free (final_value);
// 	final_key = NULL;
// 	final_value = NULL;
// 	return (var_len);
// }

// int	ft_get_expanded_len(t_minishell *mini, t_cmd *head, char *str)
// {
// 	int		j;
// 	int		len;

// 	j = 0;
// 	len = 0;
// 	while (str[j] != '\0')
// 	{
// 		if (str[j] == '\'')
// 		{
// 			j++;
// 			while (str[j] != '\'' && str[j] != '\0')
// 			{
// 				j++;
// 				len++;
// 			}
// 			if (str[j] == '\0')
// 				head->parsing_error = 1; //to check
// 			else
// 				j++;
// 		}
// 		else if (str[j] == '\"')
// 		{
// 			j++;
// 			while (str[j] != '\"' && str[j] != '\0')
// 			{
// 				if (str[j] == '$')
// 				{
// 					j++;
// 					len = len + ft_value_len(mini, &str[j]);
// 					j = j + ft_key_len(mini, &str[j]);
// 				}
// 				else
// 				{
// 					j++;
// 					len++;
// 				}
// 			}
// 			if (str[j] == '\0')
// 				head->parsing_error = 1;
// 			else
// 				j++;
// 		}
// 		else if (str[j] == '$')
// 		{
// 			j++;
// 			len = len + ft_value_len(mini, &str[j]);
// 			j = j + ft_key_len(mini, &str[j]);
// 		}
// 		else
// 		{
// 			j++;
// 			len++;
// 		}
// 	}
// 	return (len);
// }

// char	*ft_get_expanded_str(t_minishell *mini, t_cmd *head, char *str, char *temp)
// {
// 	int		j;
// 	int		len;
// 	char	*temp2;
// 	char	*temp3;

// 	j = 0;
// 	len = 0;
// 	while (str[j] != '\0')
// 	{
// 		if (str[j] == '\'')
// 		{
// 			j++;
// 			while (str[j] != '\'' && str[j] != '\0')
// 			{
// 				temp[len] = str[j];
// 				j++;
// 				len++;
// 			}
// 			if (str[j] == '\0')
// 				head->parsing_error = 1; //to check
// 			else
// 				j++;
// 		}
// 		else if (str[j] == '\"')
// 		{
// 			j++;
// 			while (str[j] != '\"' && str[j] != '\0')
// 			{
// 				if (str[j] = '$')
// 				{
// 					j++;
// 					temp3 = ft_get_env_var(mini, &str[j]);
// 					if (temp3 == NULL)
// 					{
// 						head->parsing_error = ENOMEM;
// 						continue ;
// 					}
// 					temp2 = ft_join_str(temp, temp3);
// 					if (temp2 == NULL)
// 					{
// 						head->parsing_error = ENOMEM;
// 						continue ;
// 					}
// 					free (temp3);
// 					temp3 = NULL;
// 					free (temp);
// 					temp = NULL;
// 					temp = ft_strdup(temp2);
// 					free (temp2);
// 					temp2 = NULL;
// 					len = len + ft_value_len(mini, &str[j]);
// 					j = j + ft_key_len(mini, &str[j]);
// 				}
// 				else
// 				{
// 					j++;
// 					len++;
// 				}
// 			}
// 			if (str[j] == '\0')
// 				head->parsing_error = 1;
// 			else
// 				j++;
// 		}
// 		else if (str[j] == '$')
// 		{
// 			j++;
// 			temp2 = ft_join_str(temp, ft_get_env_var(mini, &str[j]));
// 			if (temp2 == NULL)
// 			{
// 				head->parsing_error = ENOMEM;
// 				continue ;
// 			}
// 			free (temp);
// 			temp = NULL;
// 			temp = ft_strdup(temp2);
// 			free (temp2);
// 			temp2 = NULL;
// 			len = len + ft_value_len(mini, &str[j]);
// 			j = j + ft_key_len(mini, &str[j]);
// 		}
// 		else
// 		{
// 			temp[len] = str[j];
// 			j++;
// 			len++;
// 		}
// 	}
// 	temp[len] = '\0';
// 	return (temp);
// }

// int	ft_expand_variables(t_minishell *mini)
// {
// 	t_cmd	*head;
// 	char	*temp;
// 	int		final_len;
// 	int		i;
// 	int		j;

// 	head = mini->cmd;
// 	while (head != NULL)
// 	{
// 		i = 0;
// 		while (head->args[i] != NULL)
// 		{
// 			final_len = ft_get_expanded_len(mini, head, head->args[i]);
// 			if (head->parsing_error == 0)
// 			{
// 				temp = malloc(sizeof(char) * (final_len + 1));
// 				if (temp == NULL)
// 				{
// 					head->parsing_error = ENOMEM;
// 					continue ;
// 				}
// 				temp = ft_get_expanded_str(mini, head, head->args[i], temp);
// 				if (temp == NULL)
// 				{
// 					head->parsing_error = ENOMEM;
// 					continue ;
// 				}
// 				free (head->args[i]);
// 				head->args[i] = NULL;
// 				head->args[i] = ft_strdup(temp);
// 				if (head->args[i] == NULL)
// 				{
// 					head->parsing_error = ENOMEM;
// 					continue ;
// 				}
// 				free (temp);
// 				temp = NULL;
// 			}
// 			i++;
// 		}
// 	}
// }