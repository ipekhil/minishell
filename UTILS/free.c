/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:35:29 by sude              #+#    #+#             */
/*   Updated: 2025/08/25 14:51:16 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		if (current->key)
			free(current->key);
		if (current)
			free(current);
		current = next;
	}
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_all(t_data *data, int free_env_flag)
{
	free_token(data->tokens);
	data->tokens = NULL;
	free_expander(data->expander);
	data->expander = NULL;
	free_parser(data->parser);
	data->parser = NULL;
	if (free_env_flag)
	{
		free_env(data->env);
		free_array(data->char_env);
	}
}
