/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:15:27 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/23 16:07:48 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_and_write(t_data *data, char *line, int fd)
{
	int		len;
	char	*exp_line;

	len = 0;
	exp_line = NULL;
	get_len(data, line, 0, &len);
	exp_line = malloc(sizeof(char) * (len + 1));
	if (!exp_line)
		return ;
	expand_token_value(data, line, exp_line, 0);
	free(line);
	write(fd, exp_line, ft_strlen(exp_line));
	write(fd, "\n", 1);
	free(exp_line);
}

static void	read_heredoc_lines(t_data *data, char *delimiter, int fd)
{
	char	*line;

	line = NULL;
	g_signal_flag = 0;
	setup_signal_heredoc();
	while (1)
	{
		line = readline("> ");
		if (g_signal_flag == 2)
			break ;
		if (!line)
		{
			if (g_signal_flag != 2)
				printf("minishell: warning: here-document\
delimited by end-of-file (wanted '%s')\n", delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		expand_and_write(data, line, fd);
	}
	signal_dfl();
}

static int	process_single_heredoc(t_data *data, t_redirection *redir)
{
	int	heredoc_fd;

	heredoc_fd = open("heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		perror("minishell: heredoc error");
		return (-1);
	}
	read_heredoc_lines(data, redir->delimiter, heredoc_fd);
	close(heredoc_fd);
	if (g_signal_flag == 2)
	{
		if (access("heredoc_tmp", F_OK) == 0)
			unlink("heredoc_tmp");
		g_signal_flag = 0;
		return (1);
	}
	redir->filename = ft_strdup("heredoc_tmp");
	return (0);
}

static int	heredoc_process(t_data *data, t_redirection *redir)
{
	int	is_interrupted;

	while (redir)
	{
		if (redir->delimiter)
		{
			is_interrupted = process_single_heredoc(data, redir);
			if (is_interrupted != 0)
				return (is_interrupted);
		}
		redir = redir->next;
	}
	return (0);
}

void	apply_heredoc(t_data *data)
{
	t_parser	*red;
	int			std_in;
	int			is_interrupted;

	std_in = dup(STDIN_FILENO);
	red = data->parser;
	while (red)
	{
		if (red->redirection)
		{
			is_interrupted = heredoc_process(data, red->redirection);
			if (is_interrupted == 1)
			{
				red->redirection->hdoc_int = 1;
				data->last_exit_status = 130;
				break ;
			}
			else if (is_interrupted == -1)
				break ;
		}
		red = red->next;
	}
	dup2(std_in, STDIN_FILENO);
	close(std_in);
}
