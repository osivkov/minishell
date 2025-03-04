




#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int	create_pipes(t_minishell *mini, int count_cmd)
{
	int		*fd;
	int		i;
	t_cmd	*head;

	i = 0;
	head = mini->cmd;
	fd = ft_calloc(2 * ( count_cmd - 1), sizeof(int));
	if (fd == NULL)
	{
		mini->last_exit = ENOMEM;
		perror("malloc");
		return(NULL);
	}
	i = 0;
	while (i < count_cmd + 1)
	{
		if (pipe(fd + (i * 2)) < 0)
		{
			mini->last_exit = errno;
			perror(pipe);
		}
	}
	return (fd);
}
int	handle_inout_fd(t_cmd *head)
{
	int	count_cmd;

	count_cmd = 0;
	while (head = NULL)
	{
		count_cmd++;
		if (head->infile < 0)
			head->infile = STDIN_FILENO;
		if (head->outfile < 0)
			head->outfile = STDOUT_FILENO;
		head = head -> next;
	}
	return(count_cmd);
}

void	initiate_execite(t_minishell *mini, int *fd, int	count_cmd)
{
	int	i;
	int	j;
	t_cmd	*head;
	pid_t	pid;

	head = mini->cmd;
	i = 0;
	while (head != NULL)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return;
		}
		if (pid == 0)
		{
			if (head->next != NULL)
				dup2(fd[i + 1], head->outfile);
			if (i != 0)
				dup2(fd[i - 2], head->infile);
			j = 0;
			while (j < 2 * (count_cmd - 1))
			{
				close(fd[j])
				/* code */
			}
			
		}
		/* code */
	}
	
}

void execute(t_minishell *mini)
{
	int	*fd;
	int	count_cmd;

	count_cmd = handle_inout_fd(mini->cmd);
	fd = NULL;
	fd = create_pipes(mini, count_cmd);
	if (fd == NULL)
		return;
	initiate_execute(mini, fd, count_cmd);
	free(fd);
	ft_terminate_execute(mini);
}