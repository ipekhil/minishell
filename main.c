/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:00 by sude              #+#    #+#             */
/*   Updated: 2025/08/17 22:48:32 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	routine_loop(t_data *data)
{
	while (1)
	{
		g_signal_exit = data->last_exit_status;
		signal_handlers_main();
		data->line = readline("minishell> ");
		if (!data->line)
		{
			free_all(data);
			break ;
		}
		if (data->line && data->line[0] != '\0')
			add_history(data->line);
		data->last_exit_status = g_signal_exit;
		if (tokenization(data) == -1)
		{
			free_all(data);
			return ;
		}
		if (data->should_exit)
		{
			if (data->line)
			{
				free(data->line);
				data->line = NULL;
			}
			free_all(data);
			break ;
		}
		//print_parser(data->parser);
		free_token(data->tokens);
		data->tokens = NULL;
		free_expander(data->expander);
		data->expander = NULL;
		free_parser(data->parser);
		data->parser = NULL;
		free(data->line);
	}
	exit(data->last_exit_status);
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->tokens = NULL;
	data->env = NULL;
	data->expander = NULL;
	data->parser = NULL;
	data->last_exit_status = 0;
	data->should_exit = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argc;
	(void)argv;
	(void)env;
	init_data(&data);
	data.char_env = copy_env(env);
	get_env(&data.env, env);
	routine_loop(&data);
}
