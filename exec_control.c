/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:00:40 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/21 20:27:14 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pre_file_check(t_data *data, char *cmd, int *exit)
{
	struct stat	st;

	if (cmd && ft_strchr(cmd, '/'))
	{
		printf("aDBSA\n");
		if (cmd && stat(cmd, &st) == 0)
		{
			printf("1111111aDBSA\n");
			if (S_ISDIR(st.st_mode))
				handle_err_and_exit(data, NULL, ": Is a directory\n", 126);
			else if (access(cmd, X_OK) == -1)
				handle_err_and_exit(data, NULL, ": Permission denied\n", 126);
		}
		else
			handle_err_and_exit(data, NULL,
				": No such file or directory\n", 127);
	}
	else if (cmd && access(cmd, F_OK) == 0)
		handle_err_and_exit(data, cmd, ": command not found\n", 127);
	*exit = 0;
}

void	handle_fork_error(t_parser *cmds, int *pipe_fds, int *prev_read_fd)
{
	perror("minishell: fork error");
	if (*prev_read_fd != STDIN_FILENO)
		close(*prev_read_fd);
	if (cmds->next)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

void	handle_err_and_exit(t_data *data, char *cmd, const char *msg, int code)
{
	write(2, "minishell: ", 11);
	if (cmd != NULL)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	free_all(data, 1);
	exit(code);
}
