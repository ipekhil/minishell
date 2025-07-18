#include "minishell.h"

void routine_loop(t_data *data)
{
	char *line;

	line = NULL;
	while (1)
	{
		data->line = readline("minishell> ");
		if(!data->line || strcmp(data->line, "exit") == 0)
			break;
		if(data->line && data->line[0] != '\0')
			add_history(data->line);
		tokenization(data);
		free(data->line);
	}
}
void init_data(t_data *data)
{
	data->line = NULL;
}

int main(int argc, char **argv, char **env)
{
	t_data data;

	(void)argc;
	(void)argv;
	init_data(&data);
	routine_loop(&data);
}