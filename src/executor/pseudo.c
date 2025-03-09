#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>



int	ft_malloc_error(t_minishell *mini)
{
	mini->last_exit = ENOMEM;
	perror("malloc");
	return (1);
}

void	ft_error_msg(char *command, char *key, char *error_msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, command, strlen(command));
	write(STDERR_FILENO, ": ", 2);
	if (key)
	{
		write(STDERR_FILENO, key, strlen(key));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, error_msg, strlen(error_msg));
	write(STDERR_FILENO, "\n", 1);
}


char	*ft_get_env_var(t_minishell *mini, const char *var)
{
	int		i;
	size_t	len;
	char	*value;

	i = 0;
	len = strlen(var);
	while (mini->env[i])
	{
		// Проверяем, начинается ли строка с var и сразу после var стоит '='
		if (strncmp(mini->env[i], var, len) == 0 && mini->env[i][len] == '=')
		{
			value = strdup(mini->env[i] + len + 1);
			return (value);
		}
		i++;
	}
	// Если переменная не найдена, возвращаем пустую строку
	return (strdup(""));
}


int	ft_set_env_var(t_minishell *mini, char *key, char *val)
{
	int		i;
	size_t	key_len;
	char	*new_entry;

	key_len = strlen(key);
	// Выделяем память под строку "key=val"
	new_entry = malloc(key_len + 1 + strlen(val) + 1);
	if (!new_entry)
		return (1);
	strcpy(new_entry, key);
	strcat(new_entry, "=");
	strcat(new_entry, val);
	i = 0;
	while (mini->env[i])
	{
		if (strncmp(mini->env[i], key, key_len) == 0 && mini->env[i][key_len] == '=')
		{
			free(mini->env[i]);
			mini->env[i] = new_entry;
			return (0);
		}
		i++;
	}
	// Если переменная не найдена, можно добавить новую переменную.
	// Для минимальной реализации здесь можно вернуть ошибку,
	// либо реализовать расширение массива mini->env.
	free(new_entry);
	return (1);
}
char	*get_cmd_path(t_minishell *mini, const char *to_find)
{
	(void)mini;
	if (strcmp(to_find, "ls") == 0)
		return (strdup("/bin/ls"));
	return (NULL);
}

int	ft_system_cmd(t_minishell *mini, t_cmd *cmd)
{
	char	*path;

	path = get_cmd_path(mini, cmd->args[0]);
	printf("DEBUG: Путь для команды %s: %s\n", cmd->args[0], path ? path : "(null)");
	if (path == NULL)
	{
		ft_putstr_fd("command not found\n", STDERR_FILENO);
		_exit(127);
	}
	if (execve(path, cmd->args, mini->env) == -1)
	{
		mini->last_exit = errno;
		free(path);
		ft_error_msg(cmd->args[0], NULL, strerror(errno));
		_exit(mini->last_exit);
	}
	free(path);
	_exit(0);
	return (0);
}

int	ft_cd(t_minishell *mini, t_cmd *current_cmd)
{
	char	*path;
	char	*oldpwd;
	char	pwd[PATH_MAX];
	
	// Если передано больше одного аргумента, выдаём ошибку.
	if (current_cmd->args[1] && current_cmd->args[2])
	{
		mini->last_exit = 1;
		ft_error_msg("cd", NULL, "too many arguments");
		return (1);
	}
	// Если аргумента нет, или он пустой, или равен "~", берем HOME.
	if (!current_cmd->args[1] || current_cmd->args[1][0] == '\0' ||
		strcmp(current_cmd->args[1], "~") == 0)
		path = ft_get_env_var(mini, "HOME");
	// Если аргумент "-", берем OLDPWD.
	else if (strcmp(current_cmd->args[1], "-") == 0)
		path = ft_get_env_var(mini, "OLDPWD");
	// Иначе используем переданный аргумент как путь.
	else
		path = ft_strdup(current_cmd->args[1]);
	if (!path)
		return (ft_malloc_error(mini));
	// Пытаемся перейти в указанный каталог.
	if (chdir(path) != 0)
	{
		mini->last_exit = errno;
		ft_error_msg("cd", current_cmd->args[1], strerror(errno));
		free(path);
		return (1);
	}
	free(path);
	// Сохраняем старое значение PWD (текущее рабочее окружение до смены каталога).
	oldpwd = ft_get_env_var(mini, "PWD");
	if (!oldpwd)
	{
		ft_malloc_error(mini);
		return (1);
	}
	if (ft_set_env_var(mini, "OLDPWD", oldpwd) != 0)
	{
		free(oldpwd);
		ft_malloc_error(mini);
		return (1);
	}
	free(oldpwd);
	// Получаем новый текущий каталог.
	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		mini->last_exit = errno;
		ft_error_msg("cd", "getcwd", strerror(errno));
		return (1);
	}
	// Обновляем переменную PWD.
	{
		char	*newpwd = ft_strdup(pwd);
		if (!newpwd)
			return (ft_malloc_error(mini));
		if (ft_set_env_var(mini, "PWD", newpwd) != 0)
		{
			free(newpwd);
			ft_malloc_error(mini);
			return (1);
		}
		free(newpwd);
	}
	mini->last_exit = 0;
	return (0);
}


int	ft_echo(t_minishell *mini, t_cmd *cmd)
{
	int i = 1;
	(void)mini;
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i+1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

int	ft_env(t_minishell *mini, t_cmd *cmd)
{
	int i = 0;
	(void)cmd;
	while (mini->env[i])
	{
		printf("%s\n", mini->env[i]);
		i++;
	}
	return (0);
}

int	ft_export(t_minishell *mini, t_cmd *cmd)
{
	(void)mini;
	(void)cmd;
	printf("DEBUG: Встроенная команда export вызвана\n");
	return (0);
}


int	ft_pwd(t_minishell *mini, t_cmd *cmd)
{
	(void)mini;
	(void)cmd;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("getcwd");
	return (0);
}

int	ft_unset(t_minishell *mini, t_cmd *cmd)
{
	(void)mini;
	(void)cmd;
	printf("DEBUG: Встроенная команда unset вызвана\n");
	return (0);
}

int	ft_exit(t_minishell *mini, t_cmd *cmd)
{
	(void)mini;
	(void)cmd;
	printf("DEBUG: Встроенная команда exit вызвана. Завершаем работу.\n");
	exit(0);
	return (0);
}

int	is_builtin_command(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (strcmp(cmd->args[0], "cd") == 0 ||
		strcmp(cmd->args[0], "echo") == 0 ||
		strcmp(cmd->args[0], "env") == 0 ||
		strcmp(cmd->args[0], "export") == 0 ||
		strcmp(cmd->args[0], "pwd") == 0 ||
		strcmp(cmd->args[0], "unset") == 0 ||
		strcmp(cmd->args[0], "exit") == 0)
		return (1);
	return (0);
}

/*
 * begin_exec_cmd:
 * Вызывает соответствующую функцию для выполнения команды.
 * Если команда не является встроенной, вызывается функция для внешних команд.
 * (Предполагается, что функции ft_cd, ft_echo, ft_env, ft_export, ft_pwd, ft_unset, ft_exit,
 *  ft_system_cmd уже реализованы.)
 */
int	begin_exec_cmd(t_minishell *mini, t_cmd *current_cmd)
{
	if (strcmp(current_cmd->args[0], "cd") == 0)
		return (ft_cd(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "echo") == 0)
		return (ft_echo(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "env") == 0)
		return (ft_env(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "export") == 0)
		return (ft_export(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "pwd") == 0)
		return (ft_pwd(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "unset") == 0)
		return (ft_unset(mini, current_cmd));
	else if (strcmp(current_cmd->args[0], "exit") == 0)
		return (ft_exit(mini, current_cmd));
	else
		return (ft_system_cmd(mini, current_cmd));
}

/*
 * create_pipes:
 * Если в пайплайне более одной команды, выделяет массив дескрипторов размером 2*(count_cmd-1),
 * где каждая пара представляет собой два конца одного пайпа.
 * Если команд одна, возвращает NULL.
 */
int	*create_pipes(t_minishell *mini, int count_cmd)
{
	int	*fd;
	int	i, j;

	(void)mini;
	if (count_cmd <= 1)
		return (NULL);
	fd = calloc(2 * (count_cmd - 1), sizeof(int));
	if (!fd)
	{
		perror("calloc");
		return (NULL);
	}
	i = 0;
	while (i < count_cmd - 1)
	{
		if (pipe(fd + (i * 2)) < 0)
		{
			perror("pipe");
			j = 0;
			while (j < i)
			{
				close(fd[j * 2]);
				close(fd[j * 2 + 1]);
				j++;
			}
			free(fd);
			return (NULL);
		}
		i++;
	}
	return (fd);
}

/*
 * handle_inout_fd:
 * Пробегается по списку команд и устанавливает для каждой команды:
 *	– infile = STDIN_FILENO, если значение меньше 0
 *	– outfile = STDOUT_FILENO, если значение меньше 0
 * Возвращает количество команд.
 */
int	handle_inout_fd(t_cmd *head)
{
	int	count_cmd = 0;
	
	while (head != NULL)
	{
		count_cmd++;
		if (head->infile < 0)
			head->infile = STDIN_FILENO;
		if (head->outfile < 0)
			head->outfile = STDOUT_FILENO;
		head = head->next;
	}
	return (count_cmd);
}

void	pseudo_execute(t_minishell *mini)
{
	int		count_cmd;
	int		*fd;
	int		i;
	int		j;
	t_cmd	*curr;
	pid_t	pid;

	// Подсчитываем количество команд и устанавливаем стандартные дескрипторы
	count_cmd = handle_inout_fd(mini->cmd);
	printf("DEBUG: Количество команд = %d\n", count_cmd);
	if (count_cmd == 0)
	{
		printf("DEBUG: Нет команд для исполнения\n");
		return;
	}

	// Если команда одна и она встроенная, выполняем напрямую
	if (count_cmd == 1 && is_builtin_command(mini->cmd))
	{
		printf("DEBUG: Одиночная встроенная команда, выполняем напрямую\n");
		begin_exec_cmd(mini, mini->cmd);
		return;
	}

	// Если команд несколько или команда внешняя, создаём пайпы (если необходимо)
	fd = create_pipes(mini, count_cmd);
	if (fd != NULL)
	{
		printf("DEBUG: Пайпы созданы, fd: ");
		i = 0;
		while (i < 2 * (count_cmd - 1))
		{
			printf("%d ", fd[i]);
			i++;
		}
		printf("\n");
	}

	// Выполнение каждой команды в отдельном процессе
	i = 0;
	curr = mini->cmd;
	while (curr != NULL)
	{
		printf("DEBUG: Fork для команды: %s\n", curr->args[0]);
		pid = fork();
		if (pid < 0)
		{
			perror("DEBUG: Ошибка fork");
			// Закрываем пайпы
			j = 0;
			while (fd && j < 2 * (count_cmd - 1))
			{
				close(fd[j]);
				j++;
			}
			free(fd);
			return;
		}
		if (pid == 0)
		{
			// В дочернем процессе
			if (fd)
			{
				// Если не первая команда, перенаправляем STDIN
				if (i != 0)
				{
					printf("DEBUG: dup2(fd[%d], STDIN_FILENO)\n", i - 2);
					if (dup2(fd[i - 2], STDIN_FILENO) == -1)
					{
						perror("DEBUG: dup2 (stdin) error");
						_exit(1);
					}
				}
				// Если не последняя команда, перенаправляем STDOUT
				if (curr->next != NULL)
				{
					printf("DEBUG: dup2(fd[%d], STDOUT_FILENO)\n", i + 1);
					if (dup2(fd[i + 1], STDOUT_FILENO) == -1)
					{
						perror("DEBUG: dup2 (stdout) error");
						_exit(1);
					}
				}
				// Закрываем все пайп-дескрипторы в дочернем процессе
				j = 0;
				while (j < 2 * (count_cmd - 1))
				{
					close(fd[j]);
					j++;
				}
			}
			printf("DEBUG: Дочерний процесс (%s) выполняет команду\n", curr->args[0]);
			if (begin_exec_cmd(mini, curr) == 1)
				_exit(1);
			_exit(mini->last_exit);
		}
		// Родительский процесс переходит к следующей команде
		i += 2;
		curr = curr->next;
	}

	// Родительский процесс закрывает все пайп-дескрипторы
	if (fd)
	{
		j = 0;
		while (j < 2 * (count_cmd - 1))
		{
			close(fd[j]);
			j++;
		}
		free(fd);
	}

	// Родитель ожидает завершения всех дочерних процессов
	printf("DEBUG: Родитель ожидает завершения дочерних процессов\n");
	i = 0;
	while (i < count_cmd)
	{
		wait(NULL);
		i++;
	}
	printf("DEBUG: Завершили выполнение команд, возвращаемся к prompt\n");
}
