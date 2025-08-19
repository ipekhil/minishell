/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:29:29 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/19 18:29:30 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_concatenated(t_exp **nodes, t_exp *prev, t_exp *end, char *val)
{
	t_exp	*new_token;

	new_token = malloc(sizeof(t_exp));
	if (!new_token)
		return ;
	new_token->exp_value = ft_strdup(val);
	new_token->type = 5;
	new_token->concat_w_next = 2;
	new_token->next = end->next;
	end->next = new_token;
	if (!prev)
		*nodes = new_token;
	else
		prev->next = new_token;
	free(val);
}

static int	get_total_len(t_exp *start, t_exp *end)
{
	int			len;
	t_exp		*current;

	len = 0;
	current = start;
	while (current && current != end->next)
	{
		if (current->exp_value)
			len += ft_strlen(current->exp_value);
		current = current->next;
	}
	return (len);
}

static void	get_concatenated_value(char *value, t_exp *start, t_exp *end)
{
	t_exp	*current;
	int		len;
	int		i;

	i = 0;
	len = 0;
	current = start;
	while (current && current != end->next)
	{
		if (current->exp_value)
		{
			len = ft_strlen(current->exp_value);
			ft_strlcpy(&value[i], current->exp_value, len + 1);
			i += ft_strlen(current->exp_value);
		}
		current = current->next;
	}
	value[i] = '\0';
}

void	handle_merge(t_exp **nodes, t_exp *start, t_exp *end)
{
	t_exp	*current;
	t_exp	*prev;
	int		total_len;
	char	*value;

	current = *nodes;
	prev = NULL;
	total_len = get_total_len(start, end);
	value = malloc(sizeof(char) * (total_len + 1));
	get_concatenated_value(value, start, end);
	if (current != start)
	{
		while (current->next && current->next != start)
			current = current->next;
		prev = current;
	}
	add_concatenated(nodes, prev, end, value);
}
