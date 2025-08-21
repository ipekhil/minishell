/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:00 by sude              #+#    #+#             */
/*   Updated: 2025/08/21 20:27:58 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	routine_loop(t_data *data)
{
	while (1)
	{
		g_signal_flag = 0;
		signal_handlers_main();
		data->line = readline("minishell> ");
		if (!data->line)
		{
			free_all(data, 1);
			write(0, "\n", 1);
			write(1, "exit\n", 5);
			break ;
		}
		if (data->line && data->line[0] != '\0')
			add_history(data->line);
		if (g_signal_flag == 1)
			data->last_exit_status = 130;
		if (tokenization(data) == -1)
		{
			free_all(data, 1);
			return ;
		}
		if (data->should_exit)
		{
			if (data->line)
			{
				free(data->line);
				data->line = NULL;
			}
			free_all(data, 1);
			break ;
		}
		free_all(data, 0);
		free(data->line);
	}
	// free_env(data->env);
	// free_array(data->char_env);
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
