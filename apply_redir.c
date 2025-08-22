/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:29:01 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 19:19:03 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(t_data *data, t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		free_all(data, 1);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	handle_output(t_data *data, t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		free_all(data, 1);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_append(t_data *data, t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		free_all(data, 1);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_heredoc(t_data *data, t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		free_all(data, 1);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	apply_redirections(t_data *data, t_redirection *redir)
{
	while (redir)
	{
		if (redir->type == INPUT)
			handle_input(data, redir);
		else if (redir->type == OUTPUT)
			handle_output(data, redir);
		else if (redir->type == APPEND)
			handle_append(data, redir);
		else if (redir->type == HEREDOC)
			handle_heredoc(data, redir);
		redir = redir->next;
	}
}
