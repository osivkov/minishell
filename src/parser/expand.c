/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:50:09 by osivkov           #+#    #+#             */
/*   Updated: 2025/02/20 10:23:06 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
/*getenv - used to get the values of environment variables 
char	**expand_variables(char **args)
{
	int i;
	char *var_value;
	char *var_name;
	char *tmp;
	
	i = 0;
	while (args[i])
	{
		// Check if the argument contains a variable sign '$'
		if (ft_strchr(args[i], '$'))
		{
			// For simplicity, if the argument starts with '$', we handle it
			if (args[i][0] == '$')
			{
				// Get the variable name (skip the '$')
				var_name = args[i] + 1;
				// Use getenv to get the variable value from the environment
				var_value = getenv(var_name);
				if (var_value)
				{
					// Replace the original argument with the variable's value
					tmp = args[i];
					args[i] = ft_strdup(var_value);
					free(tmp);
				}
				else
				{
					// If the variable is not found, replace with an empty string
					tmp = args[i];
					args[i] = ft_strdup("");
					free(tmp);
				}
			}
			// If variable appears in the middle of the string,
			// additional parsing logic would be needed here.
		}
		i++;
	}
	return (args);
} */
