/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 19:15:47 by sude              #+#    #+#             */
/*   Updated: 2025/08/22 19:15:50 by sude             ###   ########.fr       */
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


void	free_expander(t_exp *head)
{
	t_exp	*current;
	t_exp	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->exp_value)
			free(current->exp_value);
		if (current)
			free(current);
		current = next;
	}
}

void	free_redirection(t_redirection *head)
{
	t_redirection	*current;
	t_redirection	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}

void	free_parser(t_parser *head)
{
	t_parser	*current;
	t_parser	*next;
	int			i;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->args)
		{
			i = -1;
			while (current->args[++i])
				free(current->args[i]);
			free(current->args);
		}
		if (current->redirection)
			free_redirection(current->redirection);
		free(current);
		current = next;
	}
}
