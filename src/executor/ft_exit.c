/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:28 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 10:21:11 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*will free all allocated memory, print exit on terminal and exits
if there is more than one argument, will display error and return 1*/
int	ft_exit(t_minishell *mini, t_cmd *head)
{
	int	exit_status;

	exit_status = mini->last_exit;
	if (head->args[1] != NULL)
	{
		ft_error_msg(head->args[0], NULL, "too many arguments");
		return (1);
	}
	printf("exit\n");
	free_minishell(mini);
	exit (exit_status);
}
