/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:01 by hilalipek         #+#    #+#             */
/*   Updated: 2025/07/29 20:39:02 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirection	*new_redirection_node(t_expander *current)
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

static void	add_redirection_to_parser(t_redirection **list, t_redirection *new_node)
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
int	parse_command(t_expander *start, t_expander *end, t_parser *node)
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
			current = current->next;
		}
		else if (current->type >= 0 && current->type <= 3)
		{
			printf("REDİRECTION\n");
			new_node = new_redirection_node(current);
			if (new_node)
			{
				add_redirection_to_parser(&node->redirection, new_node);
				current = current->next->next;
			}
		}
	}
	return (1);
}




void print_redirections(t_redirection *redir)
{
	while (redir)
	{
		printf("  redir type: %d, filename: %s\n", redir->type, redir->filename);
		redir = redir->next;
	}
}

void print_parser(t_parser *parser)
{
	t_parser *current = parser;
	int i;

	while (current)
	{
		printf("⎯⎯⎯⎯⎯⎯⎯⎯ Yeni Komut ⎯⎯⎯⎯⎯⎯⎯⎯\n");

		// Argümanları SADECE current->args NULL değilse yazdırın
		if (current->args) // <--- print_parser için ANA DÜZELTME
		{
			for (i = 0; current->args[i]; i++)
				printf("  arg[%d]: %s\n", i, current->args[i]);
		}
		else
			printf("  (argüman yok)\n");

		// Yönlendirmeler varsa yazdırın
		if (current->redirection)
			print_redirections(current->redirection);
		else
			printf("  (yönlendirme yok)\n");

		current = current->next;
	}
}