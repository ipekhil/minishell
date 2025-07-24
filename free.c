/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:35:29 by sude              #+#    #+#             */
/*   Updated: 2025/07/23 02:35:32 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		if (current)
			free(current);
		current = next;
	}
}

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
		if(current->key)
			free(current->key);
		if (current)
			free(current);
		current = next;
	}
}

void	free_expander(t_expander *head)
{
	t_expander	*current;
	t_expander	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if(current->exp_value)
			free(current->exp_value);
		if (current)
			free(current);
		current = next;
	}
}

void	free_all(t_data *data)
{
	free_token(data->tokens);
	free_env(data->env);
	free_expander(data->expander);
}