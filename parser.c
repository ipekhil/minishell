/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:20 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/23 19:42:12 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parser	*new_parser_node(t_exp *start, t_exp *end)
{
	t_parser	*node;
	int			count;

	node = NULL;
	node = malloc(sizeof(t_parser));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->redirection = NULL;
	node->is_error = 0;
	count = count_args(start, end);
	node->args = ft_calloc((count + 1), sizeof(char *));
	if (!node->args)
		return (NULL);
	if (!parse_command(start, end, node))
	{
		node->is_error = 1;
		return (NULL);
	}
	node->args[count] = NULL;
	node->next = NULL;
	return (node);
}

static void	add_command_to_parser(t_parser **list, t_parser *new_node)
{
	t_parser	*current;

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

static void	create_parser_list(t_data *data, t_exp *start, t_exp *current)
{
	t_parser	*new_node;

	new_node = new_parser_node(start, current);
	if (!new_node)
	{
		if (data->parser)
		{
			free_parser(data->parser);
			data->parser = NULL;
		}
		return ;
	}
	add_command_to_parser(&data->parser, new_node);
}

static void	all_tokens_parse(t_data *data, t_exp *tokens)
{
	t_exp		*current;
	t_exp		*start;

	data->parser = NULL;
	current = tokens;
	start = current;
	while (current)
	{
		if (current->type == 4)
		{
			create_parser_list(data, start, current);
			start = current->next;
		}
		current = current->next;
	}
	create_parser_list(data, start, current);
}

void	parser(t_data *data)
{
	if (!data || !data->expander)
		return ;
	if (!syntax_control(data->expander))
		return ;
	all_tokens_parse(data, data->expander);
	executor(data);
	free_parser(data->parser);
	data->parser = NULL;
}
