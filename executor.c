/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:40:29 by sude              #+#    #+#             */
/*   Updated: 2025/08/18 00:35:26 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		data->last_exit_status = cd_builtin(data, args);
	else if (ft_strcmp(args[0], "echo") == 0)
		data->last_exit_status = echo_builtin(args);
	else if (ft_strcmp(args[0], "export") == 0)
		export_builtin(data, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset_builtin(data, args);
	else if (ft_strcmp(args[0], "env") == 0)
		env_builtin(data, args);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_builtin(data, args);
	return ;
}

static void	parent_process(int *pipe_fds, int *prev_pipe, t_parser *cmd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	if (*prev_pipe != STDIN_FILENO)
		close(*prev_pipe);
	if (cmd->next)
	{
		close(pipe_fds[1]);
		*prev_pipe = pipe_fds[0];
	}
}

static void	handle_waiting(t_data *data, pid_t last_pid)
{
	int	status;
	int	sig;

	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGPIPE)
				write(2, "Broken pipe\n", 12);
			data->last_exit_status = 128 + sig;
		}
	}
	while (wait(NULL) > 0)
		;
}

static int create_pipe(t_parser *cmd, int *pipe_fds, int *prev_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe error");
			if (*prev_fd != STDIN_FILENO)
				close(*prev_fd);
			return (-1);
		}
	}
	return (0);
}

static int	execute_pipeline(t_data *data, t_parser *cmds, pid_t *last_pid, int *prev_fd)
{
	int			pipe_fds[2];

	while (cmds)
	{
		if (create_pipe(cmds, pipe_fds, prev_fd) == -1)
			return (-1);
		if ((cmds->redirection && cmds->redirection->hdoc_int == 0)
			|| !cmds->redirection)
		{
			*last_pid = fork();
			if (*last_pid < 0)
			{
				handle_fork_error(cmds, pipe_fds, prev_fd);
				return (-1);
			}
			else if (*last_pid == 0)
				child_process(data, cmds, pipe_fds, *prev_fd);
			else
				parent_process(pipe_fds, prev_fd, cmds);
		}
		cmds = cmds->next;
	}
	return (0);
}

void	executor(t_data *data)
{
	t_parser	*cmds;
	int			prev_pipe_read_fd;
	pid_t		last_pid;

	last_pid = -1;
	prev_pipe_read_fd = STDIN_FILENO;
	cmds = data->parser;
	apply_heredoc(data);
	if (!cmds)
		return ;
	if (!cmds->next && is_builtin(cmds->args[0]) && !cmds->redirection)
	{
		execute_builtin(data, cmds->args);
		return ;
	}
	if (execute_pipeline(data, cmds, &last_pid, &prev_pipe_read_fd) == -1)
		return ;
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	handle_waiting(data, last_pid);
	if (access("heredoc_tmp", F_OK) == 0)
		unlink("heredoc_tmp");
}
