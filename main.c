/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:00 by sude              #+#    #+#             */
/*   Updated: 2025/07/28 15:34:48 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	routine_loop(t_data *data)
{
	while (1)
	{
		data->line = readline("minishell> ");
		if (!data->line || strcmp(data->line, "exit") == 0)
		{
			if (data->line)
				free(data->line);
			free_all(data);
			break ;
		}
		if (data->line && data->line[0] != '\0')
			add_history(data->line);
		if (tokenization(data) == -1)
			return ;
		//print_parser(data->parser);
		free_token(data->tokens);
		data->tokens = NULL;
		free_expander(data->expander);
		data->expander = NULL;
		free_parser(data->parser);
		data->parser = NULL;
		free(data->line);
	}
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->tokens = NULL;
	data->env = NULL;
	data->expander = NULL;
	data->parser = NULL;
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argc;
	(void)argv;
	(void)env;
	init_data(&data);
	get_env(&data.env, env);
	routine_loop(&data);
}
