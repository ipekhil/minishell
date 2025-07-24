#include "minishell.h"

t_parser *new_parser_node(t_expander *start, t_expander *end)
{
	t_expander *current;
	int	count;

	count = 0;
	current = start;
	while (current != end)
	{
		if (current->type == 5)
			count++;
		current = current->next;
	}
	t_parser *node = malloc(sizeof(t_parser));
	if (!node)
		return (NULL);
	node->args = malloc(sizeof(char *) * (count + 1));
	if (!node->args)
		return (NULL);
	count = 0;
	current = start;
	while (current != end)
	{
		if (current->type == 5)
		{
			node->args[count] = current->exp_value;
			printf("node->args[%d]: %s\n", count, node->args[count]);
			count++;
		}
		current = current->next;
	}
	node->args[count] = NULL;
	node->next = NULL;
	return (node);
}

void print_parser(t_parser *parser)
{
	t_parser *current = parser;
	int i;

	while (current)
	{
		printf("⎯⎯⎯⎯⎯⎯⎯⎯ New Command ⎯⎯⎯⎯⎯⎯⎯⎯\n");
		if (current->args)
		{
			for (i = 0; current->args[i]; i++)
				printf("  arg[%d]: %s\n", i, current->args[i]);
		}
		else
			printf("  (no arguments)\n");
		current = current->next;
	}
}

void add_command_to_parser(t_parser **list, t_parser *new_node)
{
	t_parser *current;

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

t_parser *all_tokens_parse(t_data *data, t_expander *tokens)
{
	t_expander *current;
	t_expander *start;
	t_parser *new_node;

	current = tokens;
	start = current;
	while (current)
	{
		printf("Current Token: %s, Type: %d\n", current->exp_value, current->type);
		if (current->type == 4)
		{
			new_node = new_parser_node(start, current);
			add_command_to_parser(&data->parser, new_node);
			start = current->next;
		}
		current = current->next;
	}
	new_node = new_parser_node(start, current);
	add_command_to_parser(&data->parser, new_node);
	return (data->parser);
}

void	parser(t_data *data)
{
	printf("parser\n");
	if (!data || !data->expander)
		return ;
	data->parser = all_tokens_parse(data, data->expander);
}
