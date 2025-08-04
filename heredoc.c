#include "minishell.h"

void heredoc(char *delimiter, int fd)
{
	char *line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			return ;
		if(!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}
