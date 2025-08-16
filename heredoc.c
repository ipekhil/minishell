#include "minishell.h"

int g_heredoc_interrupted = 0;

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	//rl_replace_line("", 0);
	close(STDIN_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_heredoc_interrupted = 1;
}

void	heredoc(t_data *data, char *delimiter, int fd)
{
	char	*line;
	int		len;
	char	*exp_line;

	line = NULL;
	exp_line = NULL;
	len = 0;
	g_heredoc_interrupted = 0;
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		len = 0;
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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	apply_heredoc(t_data *data)
{
	int				heredoc_fd;
	t_parser		*red;
	int				std_in;
	int				std_out;
	t_redirection	*redir;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	red = data->parser;
	redir = NULL;
	while (red)
	{
		if (red->redirection)
			redir = red->redirection;
		g_heredoc_interrupted = 0;
		while (redir)
		{
			if (redir->delimiter)
			{
				heredoc_fd = open("heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (heredoc_fd < 0)
				{
					perror("minishell: heredoc error");
					return ;
				}
				heredoc(data, redir->delimiter, heredoc_fd);
				close(heredoc_fd);
				if (g_heredoc_interrupted)
				{
					redir = red->redirection;
					while (redir)
					{
						redir->hdoc_int = 1;
						redir = redir->next;
					}
					if (access("heredoc_tmp", F_OK) == 0)
						unlink("heredoc_tmp");
					return ;
				}
				redir->filename = ft_strdup("heredoc_tmp");
			}
			redir = redir->next;
		}
		red = red->next;
	}
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
}
