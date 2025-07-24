/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:35:29 by sude              #+#    #+#             */
/*   Updated: 2025/07/26 00:10:06 by sude             ###   ########.fr       */
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

void	free_parser(t_parser *head)
{
	t_parser	*current;
	t_parser	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if(current->args)
			free(current->args);
		if (current)
			free(current);
		current = next;
	}
}

void	free_all(t_data *data)
{
	free_token(data->tokens);
	data->tokens = NULL;
	free_env(data->env);
	free_expander(data->expander);;
	data->expander = NULL;
	free_parser(data->parser);
	data->parser = NULL;
}