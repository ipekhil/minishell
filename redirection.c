/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:01 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/17 22:37:16 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*new_redirection_node(t_exp *current)
{
	t_redirection	*node;

	if (current->next && current->next->type == 5)
	{
		node = malloc(sizeof(t_redirection));
		if (!node)
			return (NULL);
		node->type = current->type;
		if (current->type == 0)
		{
			node->delimiter = ft_strdup(current->next->exp_value);
			node->hdoc_int = 0;
			node->filename = NULL;
		}
		else
		{
			node->filename = ft_strdup(current->next->exp_value);
			node->delimiter = NULL;
			node->hdoc_int = 0;
		}
		node->next = NULL;
		return (node);
	}
	return (NULL);
}

void	add_redir_to_parser(t_redirection **list, t_redirection *new_node)
{
	t_redirection	*current;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
