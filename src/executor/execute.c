


// #include "minishell.h"
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <errno.h>
// #include <string.h>


// static char	**get_paths(char **envp)
// {
// 	while (*envp && ft_strncmp("PATH=", *envp, 5) != 0)
// 		envp++;
// 	if (!*envp)
// 		return (NULL);
// 	return (ft_split(*envp + 5, ':'));
// }

// static char	*check_command_in_path(char **paths, char *cmd)
// {
// 	char	*temp;
// 	char	*full_path;
// 	int		i;

// 	i = 0;
// 	while (paths[i])
// 	{
// 		temp = ft_strjoin(paths[i], "/");
// 		full_path = ft_strjoin(temp, cmd);
// 		free(temp);
// 		if (access(full_path, F_OK) == 0)
// 		{
// 			ft_free_split(paths);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	ft_free_split(paths);
// 	return (NULL);
// }

// static char	*find_command_path(char *cmd, char **envp)
// {
// 	char	**paths;
// 	char	*full_path;

// 	paths = get_paths(envp);
// 	if (!paths)
// 		return (NULL);
// 	full_path = check_command_in_path(paths, cmd);
// 	return (full_path);
// }



// /*
//  * Псевдо-функция execute:
//  * - Принимает shell для того, чтобы можно было выставлять last_exit.
//  * - Принимает cmd_list (список команд), хотя в простейшем случае
//  *   можно взять только первую команду (cmd_list->args).
//  */
// int execute(t_minishell *shell, t_cmd *cmd)
// {

//     t_cmd    *cmd = args[0];
//     char    *cmd_path;
//     pid_t   pid;
//     int     status;

//     if (!cmd)
//         return (0);

//     // Если в команде есть '/', проверяем путь напрямую
//     if (ft_strchr(cmd, '/'))
//     {
//         // Сразу берем cmd как путь
//         if (access(cmd, X_OK) == 0)
//             cmd_path = ft_strdup(cmd);
//         else
//             cmd_path = NULL;
//     }
//     else
//     {
//         // Ищем команду в PATH
//         cmd_path = find_command_path(cmd, shell->env); 
//         // здесь shell->env — это ваш массив окружения
//         // (может называться shell->envp)
//     }

//     // Если так и не нашли, выводим ошибку:
//     if (!cmd_path)
//     {
//         // Можно проверить, не было ли доступа (EACCES), 
//         // но если просто не нашли => "command not found"
//         ft_putstr_fd(cmd, 2);
//         ft_putstr_fd(": command not found\n", 2);
//         shell->last_exit = 127;
//         return (127);
//     }

//     // Создаем новый процесс
//     pid = fork();
//     if (pid < 0)
//     {
//         perror("fork");
//         shell->last_exit = 1;
//         free(cmd_path);
//         return (1);
//     }
//     else if (pid == 0)
//     {
//         // Дочерний процесс
//         execve(cmd_path, args, shell->env);
//         // Если execve не сработал
//         perror(cmd_path);
//         // Ошибки: EACCES => 126, ENOENT => 127, 
//         // или можно использовать одно число для всех случаев.
//         _exit(126);
//     }
//     else
//     {
//         // Родитель ждет
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status))
//             shell->last_exit = WEXITSTATUS(status);
//         else if (WIFSIGNALED(status))
//             shell->last_exit = 128 + WTERMSIG(status);
//     }
//     free(cmd_path);
//     return (shell->last_exit);
// }
