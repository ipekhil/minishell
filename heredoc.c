#include "minishell.h"

static void	get_len2(t_data *data, char *t_value, int i, int *len)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	while (t_value[i] != '\0')
	{
		if (t_value[i] == '$')
		{
			i++;
			key = extract_key(&(t_value[i]));
			if (key[0] == '\0')
				*len += 1;
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			if (value)
				*len += ft_strlen(value);
			free(key);
		}
		else
		{
			(*len)++;
			i++;
		}
	}
}

static void	append_value2(char *exp_value, char *value, int *a_index)
{
	int	i;

	i = 0;
	if (!value)
		return ;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}
static void	expand_token_value2(t_data *data, char *line, char *exp_line, int i)
{
	char	*key;
	char	*value;
	int		a_index;

	a_index = 0;
	key = NULL;
	value = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			i++;
			key = extract_key(&line[i]);
			if (key[0] == '\0')
				exp_line[a_index++] = '$';
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			append_value2(exp_line, value, &a_index);
			free(key);
		}
		else
			exp_line[a_index++] = line[i++];
	}
	exp_line[a_index] = '\0';
}

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
		get_len2(data, line, 0, &len);
		printf("Heredoc line length: %d\n", len);
		exp_line = malloc(sizeof(char) * (len + 1));
		if (!exp_line)
			return ;
		expand_token_value2(data, line, exp_line, 0);
		free(line);
		printf("Heredoc expanded line: %s\n", exp_line);
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
