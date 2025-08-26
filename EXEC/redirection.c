/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:01 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/26 16:08:09 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirection	*new_redirection_node(t_exp *current)
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

static void	add_redir_to_parser(t_redirection **list, t_redirection *new_node)
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

int	count_args(t_exp *start, t_exp *end)
{
	t_exp	*current;
	int		count;

	count = 0;
	current = start;
	while (current != end)
	{
		if (current->type == 5)
			count++;
		current = current->next;
	}
	return (count);
}

int	parse_command(t_exp *start, t_exp *end, t_parser *node)
{
	t_exp			*current;
	t_redirection	*new_node;
	int				count;

	count = 0;
	current = start;
	new_node = NULL;
	while (current != end)
	{
		if (current->type == 5)
		{
			node->args[count] = ft_strdup(current->exp_value);
			count++;
			current = current->next;
		}
		else if (current->type >= 0 && current->type <= 3)
		{
			new_node = new_redirection_node(current);
			current = current->next;
			if (new_node)
			{
				add_redir_to_parser(&node->redirection, new_node);
				current = current->next;
			}
		}
	}
	return (1);
}
