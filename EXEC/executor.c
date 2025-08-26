/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:40:29 by sude              #+#    #+#             */
/*   Updated: 2025/08/26 15:48:15 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	status = 0;
	sig = 0;
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

static int	create_pipe(t_parser *cmd, int *pipe_fds, int *prev_fd)
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

static int	exec_pipeline(t_data *data, t_parser *cmds, pid_t *l_pid, int *p_fd)
{
	int	pipe_fds[2];

	if (heredoc_was_interrupted(cmds))
		return (0);
	while (cmds)
	{
		if (create_pipe(cmds, pipe_fds, p_fd) == -1)
			return (-1);
		*l_pid = fork();
		if (*l_pid < 0)
		{
			handle_fork_error(cmds, pipe_fds, p_fd);
			return (-1);
		}
		else if (*l_pid == 0)
			child_process(data, cmds, pipe_fds, *p_fd);
		else
			parent_process(pipe_fds, p_fd, cmds);
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
	if (exec_pipeline(data, cmds, &last_pid, &prev_pipe_read_fd) == -1)
		return ;
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	handle_waiting(data, last_pid);
	if (access("heredoc_tmp", F_OK) == 0)
		unlink("heredoc_tmp");
}
