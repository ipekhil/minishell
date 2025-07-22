#include "minishell.h"

void routine_loop(t_data *data)
{
	while (1)
	{
		data->line = readline("minishell> ");
		if(!data->line || strcmp(data->line, "exit") == 0)
			break;
		if(data->line && data->line[0] != '\0')
		{
			add_history(data->line);
			tokenization(data);
			if (data->token_count > 0)
			{
				if (is_builtin(data->tokens[0].value))
					execute_builtin(data);
				else
					printf("Command not found: %s\n", data->tokens[0].value);
			}
		}
		free(data->line);
	}
}
void init_data(t_data *data, char **env)
{
	data->line = NULL;
	data->tokens = NULL;
	data->token_count = 0;
	data->env = env;
}

int main(int argc, char **argv, char **env)
{
	t_data data;

	(void)argc;
	(void)argv;
	(void)env;
	init_data(&data, copy_env(env));
	routine_loop(&data);
	return (0);
}