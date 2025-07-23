/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:00 by sude              #+#    #+#             */
/*   Updated: 2025/07/23 02:31:01 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	routine_loop(t_data *data)
{
	while (1)
	{
		data->line = readline("minishell> ");
		if (!data->line || strcmp(data->line, "exit") == 0)
			break ;
		if (data->line && data->line[0] != '\0')
			add_history(data->line);
		if (tokenization(data) == -1)
			return ;
		free_token(data->tokens);
		data->tokens = NULL;
		free(data->line);
	}
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->tokens = NULL;
	data->env = NULL;
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
