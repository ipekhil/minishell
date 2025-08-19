/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:30:26 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/19 19:08:34 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		len;

	if (!dir || !cmd)
		return (NULL);
	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	**search_in_env(char **env, char **paths)
{
	int		i;
	char	*path_env;

	path_env = NULL;
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
	return (paths);
}

char	*find_command_path(char *cmd, char **env)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	paths = NULL;
	paths = search_in_env(env, paths);
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

void	not_found_path(t_data *data, t_parser *cmd, char **temp_env)
{
	char	**args;

	args = cmd->args;
	if (cmd->redirection && !args[0])
	{
		free_array(temp_env);
		free_all(data, 1);
		exit(0);
	}
	else
	{
		free_array(temp_env);
		handle_err_and_exit(data, args[0], "command not found\n", 127);
	}
}

void	execute_command_in_child(t_data *data, t_parser *cmd)
{
	char	*path;
	char	**temp_env;
	char	**args;

	args = cmd->args;
	temp_env = convert_env_to_array(data->env);
	if (!temp_env)
	{
		free_all(data, 1);
		exit(1);
	}
	path = find_command_path(args[0], temp_env);
	if (!path)
		not_found_path(data, cmd, temp_env);
	else if (execve(path, args, data->char_env) == -1)
	{
		perror("execve");
		free(path);
		free_array(temp_env);
		free_all(data, 1);
		exit(127);
	}
}

void	child_process(t_data *data, t_parser *cmd, int *pipe_fds, int prev_pipe)
{
	pre_file_check(data, cmd->args[0], &data->last_exit_status);
	if (data->last_exit_status != 0)
		return ;
	setup_child_signals();
	if (prev_pipe != STDIN_FILENO)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (cmd->next)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	if (cmd->redirection)
		apply_redirections(data, cmd->redirection);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(data, cmd->args);
		free_all(data, 1);
		exit(0);
	}
	else
		execute_command_in_child(data, cmd);
}
