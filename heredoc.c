#include "minishell.h"

void	heredoc(t_data *data, char *delimiter, int fd)
{
	char	*line;
	int		len;
	char	*exp_line;

	line = NULL;
	exp_line = NULL;
	len = 0;
	while (1)
	{
		len = 0;
		line = readline("> ");
		if (!line)
			return ;
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
}

void	handle_heredoc(t_data *data)
{
	int				heredoc_fd;
	t_redirection	*red;

	red = data->parser->redirection;
	while (red)
	{
		if (red->delimiter)
		{
			heredoc_fd = open("heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (heredoc_fd < 0)
			{
				perror("minishell: heredoc error");
				return ;
			}
			heredoc(data, red->delimiter, heredoc_fd);
			close(heredoc_fd);
			red->filename = ft_strdup("heredoc_tmp");
			printf("Heredoc content saved to %s\n", red->filename);
		}
		red = red->next;
	}
}
