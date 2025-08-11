/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:46:46 by sude              #+#    #+#             */
/*   Updated: 2025/08/11 14:46:02 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_merged(t_exp *start)
{
	t_exp	*tmp;

	tmp = NULL;
	while (start && start->concat_w_next != 2)
	{
		if (start->next)
			tmp = start->next;
		free(start->exp_value);
		free(start);
		start = tmp;
	}
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
	printf("----------------------------------------\n");
	t_exp *debug = data->expander;
	while(debug)
	{
		printf("CONC Value: %s TYPE: %d FLAG:%d\n", debug->exp_value, debug->type, debug->concat_w_next);
		debug = debug->next;
	}
	parser(data);
}
