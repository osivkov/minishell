/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:20 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 10:04:39 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//malloc error, will return ENOMEM
int	ft_malloc_error(t_minishell *mini)
{
	mini->last_exit = ENOMEM;
	ft_error_msg("malloc", NULL, strerror(ENOMEM));
	return (ENOMEM);
}
