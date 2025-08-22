/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:30:26 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 19:27:20 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_command_in_child(t_data *data, t_parser *cmd)
{
	char	*path;
	char	**temp_env;
	char	**args;

	args = cmd->args;
	if (cmd->redirection && !args[0])
	{
		free_all(data, 1);
		exit(0);
	}
	temp_env = convert_env_to_array(data->env);
	if (!temp_env)
	{
		free_all(data, 1);
		exit(1);
	}
	path = find_command_path(data, args[0], temp_env);
	if (execve(path, args, data->char_env) == -1)
	{
		perror("minishell");
		free(path);
		free_array(temp_env);
		free_all(data, 1);
		exit(127);
	}
}

void	child_process(t_data *data, t_parser *cmd, int *pipe_fds, int prev_pipe)
{
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
