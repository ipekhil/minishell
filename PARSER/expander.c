/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:32:19 by sude              #+#    #+#             */
/*   Updated: 2025/08/25 20:23:07 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_with_variables(t_data *data, t_token *token, t_exp *node)
{
	int	len;

	len = 0;
	get_len(data, token->value, 0, &len);
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
		return ;
	expand_token_value(data, token->value, node->exp_value, 0);
}

static void	set_exp_var(t_data *data, t_token *token, t_exp *n_node, int exp_fl)
{
	if (token->type <= 9 && token->type >= 6)
		n_node->type = 5;
	else
		n_node->type = token->type;
	if ((token->type == 9 || token->type == 8) && exp_fl)
		expand_with_variables(data, token, n_node);
	else
	{
		n_node->exp_value = ft_strdup(token->value);
	}
	n_node->concat_w_next = token->concat_w_next;
	n_node->next = NULL;
}

static void	add_exp_node(t_data *data, t_token *token, int expand_flag)
{
	t_exp	*new_node;
	t_exp	*tmp;

	tmp = NULL;
	new_node = malloc(sizeof(t_exp));
	if (!new_node)
		return ;
	set_exp_var(data, token, new_node, expand_flag);
	if (!data->expander)
		data->expander = new_node;
	else
	{
		tmp = data->expander;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	expander(t_data *data)
{
	t_token	*tmp;
	int		expand_flag;
	int		len;

	tmp = data->tokens;
	data->expander = NULL;
	expand_flag = 1;
	while (tmp)
	{
		len = 0;
		get_len(data, tmp->value, 0, &len);
		if (len != -1)
		{
			add_exp_node(data, tmp, expand_flag);
			expand_flag = 1;
			if (tmp->type == 0 && tmp->next)
				expand_flag = 0;
		}
		tmp = tmp->next;
	}
	concatenator(data);
}
