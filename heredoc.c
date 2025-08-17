#include "minishell.h"

int	g_heredoc_interrupted = 0;

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close(STDIN_FILENO);
	rl_on_new_line();
	g_heredoc_interrupted = 1;
}

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

void	read_heredoc_lines(t_data *data, char *delimiter, int fd)
{
	char	*line;

	line = NULL;
	g_heredoc_interrupted = 0;
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (!g_heredoc_interrupted)
				printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", delimiter);
			break ;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		expand_and_write(data, line, fd);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
	if (g_heredoc_interrupted)
	{
		if (access("heredoc_tmp", F_OK) == 0)
			unlink("heredoc_tmp");
		return (1);
	}
	redir->filename = ft_strdup("heredoc_tmp");
	return (0);
}

static int	heredoc_process(t_data *data, t_redirection *redir)
{
	int	is_interrupted;

	g_heredoc_interrupted = 0;
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
