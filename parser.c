#include "minishell.h"

int	count_args(t_expander *start, t_expander *end)
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
	return (count);
}

t_redirection	*new_redirection_node(t_expander *current)
{
	t_redirection	*node;

	if (current->next && current->next->type == 5)
	{
		node = malloc(sizeof(t_redirection));
		if (!node)
			return (NULL);
		node->type = current->type;
		node->filename = ft_strdup(current->next->exp_value);
		node->next = NULL;
		return (node);
	}
	return (NULL);
}

void	add_redirection_to_parser(t_redirection **list, t_redirection *new_node)
{
	t_redirection *current;

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

void	parse_command(t_expander *start, t_expander *end, t_parser *node)
{
	t_expander *current;
	t_redirection *new_node;
	int	count;

	count = 0;
	current = start;
	while (current != end)
	{
		if (current->type == 5)
		{
			node->args[count] = ft_strdup(current->exp_value);
			printf("node->args[%d]: %s\n", count, node->args[count]);
			count++;
		}
		else if (current->type >= 0 && current->type <= 3)
		{
			printf("REDİRECTION\n");
			new_node = new_redirection_node(current);
			if (new_node)
			{
				add_redirection_to_parser(&node->redirection, new_node);
				current = current->next;
			}
		}
		current = current->next;
	}
}

t_parser *new_parser_node(t_expander *start, t_expander *end)
{
	t_parser	*node;
	int			count;

	node = NULL;
	node = malloc(sizeof(t_parser));
	if (!node)
		return (NULL);
	node->redirection = NULL;
	node->args = NULL;
	count = count_args(start, end);
	node->args = ft_calloc((count + 1), sizeof(char *));
	if (!node->args)
		return (NULL);
	parse_command(start, end, node);
	//node->args[count] = NULL;
	node->next = NULL;
	return (node);
}

// void print_redirections(t_redirection *redir)
// {
// 	while (redir)
// 	{
// 		printf("  redir type: %d, filename: %s\n", redir->type, redir->filename);
// 		redir = redir->next;
// 	}
// }

// void print_parser(t_parser *parser)
// {
// 	t_parser *current = parser;
// 	int i;

// 	while (current)
// 	{
// 		printf("⎯⎯⎯⎯⎯⎯⎯⎯ Yeni Komut ⎯⎯⎯⎯⎯⎯⎯⎯\n");

// 		// Argümanları SADECE current->args NULL değilse yazdırın
// 		if (current->args) // <--- print_parser için ANA DÜZELTME
// 		{
// 			for (i = 0; current->args[i]; i++)
// 				printf("  arg[%d]: %s\n", i, current->args[i]);
// 		}
// 		else
// 			printf("  (argüman yok)\n");

// 		// Yönlendirmeler varsa yazdırın
// 		if (current->redirection)
// 			print_redirections(current->redirection);
// 		else
// 			printf("  (yönlendirme yok)\n");

// 		current = current->next;
// 	}
// }


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

void all_tokens_parse(t_data *data, t_expander *tokens)
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
}

void	parser(t_data *data)
{
	if (!data || !data->expander)
		return ;
	all_tokens_parse(data, data->expander);
}
