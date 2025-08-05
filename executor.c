/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:40:29 by sude              #+#    #+#             */
/*   Updated: 2025/08/05 05:18:53 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	if (!dest || !src)
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	dest_len;
	int	i;

	if (!dest || !src)
		return (dest);

	dest_len = ft_strlen(dest);
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_data *data, char **args)
{
	if (!args)
		return ;
	if (ft_strcmp(args[0], "pwd") == 0)
		data->last_exit_status = pwd_builtin();
	else if (ft_strcmp(args[0], "cd") == 0)
		data->last_exit_status = cd_builtin(args, &data->char_env);
	else if (ft_strcmp(args[0], "echo") == 0)
		data->last_exit_status = echo_builtin(args);
	else if (ft_strcmp(args[0], "export") == 0)
		export_builtin(data, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset_builtin(data, args);
	else if (ft_strcmp(args[0], "env") == 0)
		env_builtin(data->env);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_builtin(data, args);
	printf("last_exit_status: %d\n", data->last_exit_status);	
	return ;
}

char	*create_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	*find_command_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = NULL;
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = create_full_path(paths[i], cmd);
		if (access(full_path, F_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

void	execute_command_in_child(t_data *data, char **args)
{
	char	*path;

	path = find_command_path(args[0], data->char_env);
	if (!path)
	{
		printf("minishell: %s: command not found\n", args[0]);
		exit(127);
	}
	if (execve(path, args, data->char_env) == -1)
	{
		perror("execve");
		free(path);
		exit(127);
	}
}

void    executor(t_data *data)
{
    t_parser    *cmds;
    int         pipe_fds[2];
    int         prev_pipe_read_fd;
    pid_t       last_pid = -1;
    int         status;

	prev_pipe_read_fd = STDIN_FILENO;
	cmds = data->parser;
	if (!cmds)
		return ;
	if (!cmds->next && is_builtin(cmds->args[0]))
	{
		execute_builtin(data, cmds->args);
		// builtin dönüş değer data->last_exit_status'a kaydet
		return ;
	}
	while (cmds)
	{
		if (cmds->next)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("minishell: pipe error");
				if (prev_pipe_read_fd != STDIN_FILENO)
					close(prev_pipe_read_fd);
				return ;
			}
		}
		last_pid = fork();
		if (last_pid < 0)
		{
			perror("minishell: fork error");
			if (prev_pipe_read_fd != STDIN_FILENO)
				close(prev_pipe_read_fd);
			if (cmds->next)
			{
				close(pipe_fds[0]);
				close(pipe_fds[1]);
			}
			return ;
		}
		else if (last_pid == 0)
		{
			if (prev_pipe_read_fd != STDIN_FILENO)
			{
				dup2(prev_pipe_read_fd, STDIN_FILENO);
				close(prev_pipe_read_fd);
			}
			if (cmds->next)
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[0]);
				close(pipe_fds[1]);
			}
			// apply_redirections(cmds->redirections);
			if (is_builtin(cmds->args[0]))
			{
				execute_builtin(data, cmds->args);
				exit(0);
			}
			else
			{
				execute_command_in_child(data, cmds->args);
			}
			exit(1);
		}
		else
		{
			if (prev_pipe_read_fd != STDIN_FILENO)
				close(prev_pipe_read_fd);
			if (cmds->next)
			{
				close(pipe_fds[1]);
				prev_pipe_read_fd = pipe_fds[0];
			}
		}
		cmds = cmds->next;
	}
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->last_exit_status = 128 + WTERMSIG(status);
	}
	while (wait(NULL) > 0);
}
