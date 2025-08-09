/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:46:46 by sude              #+#    #+#             */
/*   Updated: 2025/08/08 21:23:45 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_merged(t_exp *start)
{
	t_exp *tmp;

	tmp = NULL;
	while(start && start->concat_w_next != 2)
	{
		if(start->next)
			tmp = start->next;
		free(start->exp_value);
		free(start);
		start = tmp;
	}
}

void	add_concatenated(t_exp **tokens, t_exp *prev, t_exp *end, char *value)
{
	t_exp	*new_token;

	new_token = malloc(sizeof(t_exp));
	if (!new_token)
		return ;
	new_token->exp_value = ft_strdup(value);
	new_token->type = 5;
	new_token->concat_w_next = 2;
	new_token->next = end->next;
	end->next = new_token;
	if (!prev)
		*tokens = new_token;
	else
		prev->next = new_token;
	free(value);
}

int	ret_num_of_concat(t_exp *current)
{
	int			num_of_concat;
	t_exp		*tmp;

	tmp = current;
	num_of_concat = 0;
	while (tmp)
	{
		if (tmp->concat_w_next == 1)
			num_of_concat++;
		else
			break ;
		tmp = tmp->next;
	}
	return (num_of_concat);
}

int	get_total_len(t_exp *start, t_exp *end)
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

void	get_concatenated_value(char *value, t_exp *start, t_exp *end)
{
	t_exp	*current;
	int		i;

	i = 0;
	current = start;
	while (current && current != end->next)
	{
		if (current->exp_value)
		{
			ft_strlcpy(&value[i], current->exp_value, ft_strlen(current->exp_value) + 1);
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


void	concatenator(t_data *data)
{
	t_exp	*current;
	t_exp	*start;
	t_exp	*end;
	int		num_of_concat;

	current = data->expander;
	num_of_concat = 0;
	while (current)
	{
		start = current;
		num_of_concat = ret_num_of_concat(current);
		if (num_of_concat != 0)
		{
			end = start;
			while (num_of_concat-- > 0 && end->next)
				end = end->next;
			handle_merge(&(data->expander), start, end);
			current = end->next;
			free_merged(start);
		}
		else
			current = current->next;
	}
	/*
	printf("----------------------------------------\n");
	t_exp *debug = data->expander;
	while(debug)
	{
		printf("CONC Value: %s TYPE: %d FLAG:%d\n", debug->exp_value, debug->type, debug->concat_w_next);
		debug = debug->next;
	}
	*/
	parser(data);
}
