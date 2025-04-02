/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:28 by dsewlia           #+#    #+#             */
/*   Updated: 2025/04/02 16:24:39 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	ft_check_exit_error(t_minishell *mini, t_cmd *head)
{
	int		i;

	i = -1;
	if (head->args[1][0] == '+' || head->args[1][0] == '-')
		i++;
	while (head->args[1][++i] != '\0')
	{
		if (head->args[1][i] < '0' || head->args[1][i] > '9')
		{
			printf("exit\n");
			ft_error_msg(head->args[0], head->args[1], \
				"numberic argument required");
			mini->last_exit = 2;
			return (2);
		}
	}
	if (head->args[2] != NULL && head->args[2][0] != '\0')
	{
		ft_error_msg(head->args[0], NULL, "too many arguments");
		mini->last_exit = 1;
		return (1);
	}
	return (0);
}

int	ft_exit(t_minishell *mini, t_cmd *head)
{
	int		exit_status;

	exit_status = mini->last_exit;
	if (head->args[1] == NULL || head->args[1][0] == '\0')
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		free_minishell(mini);
		exit(exit_status);
	}
	exit_status = ft_check_exit_error(mini, head);
	if (exit_status == 1)
		return (exit_status);
	if (exit_status == 0)
	{
		exit_status = ft_atoi(head->args[1]);
		while (exit_status < 0)
			exit_status += 256;
		exit_status = exit_status % 256;
		mini->last_exit = exit_status;
		if (isatty(STDIN_FILENO))
			printf("exit\n");
	}
	free_minishell(mini);
	exit(exit_status);
}
