/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsewlia <dsewlia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:50:06 by dsewlia           #+#    #+#             */
/*   Updated: 2025/03/24 10:23:43 by dsewlia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//waits for child process and updates last exit in t_minishell
void	ft_kill_child(t_minishell *mini, int count_cmd)
{
	int		i;
	pid_t	child_pid;
	int		status;

	i = 0;
	while (i < count_cmd)
	{
		child_pid = wait(&status);
		if (child_pid == -1)
		{
			perror("wait");
			break ;
		}
		if (WIFEXITED(status))
			mini->last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			mini->last_exit = 128 + WTERMSIG(status);
		}
		i++;
	}
}

//duplicates infile fd in pipes communication
void	ft_handle_infile(int *fd, t_cmd *head, int i)
{
	if (head->infile != STDIN_FILENO)
	{
		if (dup2(head->infile, STDIN_FILENO) == -1)
		{
			perror("dup2 (file-in)");
			exit (1);
		}
		close (head->infile);
	}
	else if (i != 0)
	{
		if (dup2(fd[i - 2], STDIN_FILENO) == -1)
		{
			perror("dup2 (pipe-in)");
			exit (1);
		}
	}
}

//duplicates outfile fd in pipes communication
void	ft_handle_outfile(int *fd, t_cmd *head, int i)
{
	if (head->outfile != STDOUT_FILENO)
	{
		if (dup2(head->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2 (file-out)");
			exit (1);
		}
		close (head->outfile);
	}
	else if (head->next != NULL)
	{
		if (dup2(fd[i + 1], STDOUT_FILENO) == -1)
		{
			perror("dup2 (pipe-out)");
			exit (1);
		}
	}
}

//initiates child process
void	ft_init_child(t_minishell *mini, int *fd, t_cmd *head, int i)
{
	int		count_cmd;
	t_cmd	*temp;
	int		j;

	temp = mini->cmd;
	count_cmd = 0;
	while (temp != NULL)
	{
		count_cmd++;
		temp = temp->next;
	}
	ft_handle_infile(fd, head, i);
	ft_handle_outfile(fd, head, i);
	j = -1;
	while (++j < (2 * count_cmd))
		close(fd[j]);
	begin_builtin(mini, head);
}

/*creates pipes for communication
will return pointer to the fd array on success
will print error message and return NULL on error*/
int	*create_pipes(t_minishell *mini, int count_cmd)
{
	int		*fd;
	int		i;

	i = 0;
	fd = ft_calloc(2 * (count_cmd), sizeof(int));
	if (fd == NULL)
	{
		mini->last_exit = errno;
		return (perror("malloc"), NULL);
	}
	while (i < count_cmd)
	{
		if (pipe(fd + (i * 2)) < 0)
		{
			mini->last_exit = errno;
			free (fd);
			return (perror("pipe"), NULL);
		}
		i++;
	}
	return (fd);
}
