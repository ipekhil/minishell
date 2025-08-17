#include "minishell.h"

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
	while (current != end)
	{
		if (current->type == 5)
		{
			node->args[count++] = ft_strdup(current->exp_value);
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
