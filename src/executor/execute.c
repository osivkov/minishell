/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:13:52 by dsewlia           #+#    #+#             */
/*   Updated: 2025/04/02 09:38:33 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

int	handle_inout_fd(t_cmd *head)
{
	int		count_cmd;

	count_cmd = 0;
	while (head != NULL && head->args != NULL && \
		head->args[0] != NULL && head->args[0][0] != '\0')
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

void	initiate_execute(t_minishell *mini, int *fd, int count_cmd)
{
	t_cmd			*head;
	pid_t			pid;
	int				i;

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

void	mini_terminal(t_minishell *mini, int count_cmd)
{
	int				*fd;
	fd = create_pipes(mini, count_cmd);
	if (fd == NULL)
		return ;
	initiate_execute(mini, fd, count_cmd);
	free (fd);

}
void	execute(t_minishell *mini)
{
	int		count_cmd;

	count_cmd = handle_inout_fd(mini->cmd);
	if (count_cmd == 0)
		return ;
	if (count_cmd == 1 && check_builtin(mini->cmd) == 0)
	{
		mini->last_exit = begin_builtin(mini, mini->cmd);
		return ;
	}
	mini_terminal(mini, count_cmd);
	return ;
}
