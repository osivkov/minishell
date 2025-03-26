/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:13:52 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 18:48:22 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

/*execute.c*/
//resets infile and outfile in cmd structs if there is no redirection
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

//begins execution.
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

/*execute.c*/
/*will begin single buitin command. will check if there are infile
and outfile that is to be duplicated and then initiates the command
will return the exit status of the command*/
// int	begin_builtin_single(t_minishell *mini, t_cmd *head, int dup_flag)
// {
// 	int		fd[2];

// 	if (head->outfile != STDOUT_FILENO || head->infile != STDIN_FILENO)
// 		dup_flag = 1;
// 	if (dup_flag == 1)
// 	{
// 		if (pipe(fd) < 0)
// 		{
// 			mini->last_exit = errno;
// 			return (perror("pipe"), mini->last_exit);
// 		}
// 		if (head->infile != 0 && dup2(head->infile, STDIN_FILENO) == -1)
// 			return (perror("dup2 (file-in)"), errno);
// 		if (head->outfile != 1 && dup2(head->outfile, STDOUT_FILENO) == -1)
// 			return (perror("dup2 (file-out)"), errno);
// 		close (fd[0]);
// 		close (fd[1]);
// 	}
// 	mini->last_exit = begin_builtin(mini, head);
// 	if (dup_flag == 1)
// 	{
// 		close (fd[0]);
// 		close (fd[1]);
// 	}
// 	return (mini->last_exit);
// }

void	mini_terminal(t_minishell *mini, int count_cmd)
{
	int				*fd;
	// struct termios	term_set;

	// if (tcgetattr(STDIN_FILENO, &term_set) == -1)
	// {
	// 	perror("tcgetattr error");
	// 	free_minishell(mini);
	// 	exit (1);
	// }
	fd = create_pipes(mini, count_cmd);
	if (fd != NULL)
	{
		initiate_execute(mini, fd, count_cmd);
		free (fd);
	}
// 	if (tcsetattr(STDIN_FILENO, TCSANOW, &term_set) == -1)
// 	{
// 		perror("tcsettr error");
// 		free_minishell(mini);
// 		exit (1);
// 	}
}

/*execute.c*/
/*will execute all commands in mini->cmd, will set last_exit
will free cmd and args in cmd and then return*/
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
