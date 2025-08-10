/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:32:19 by sude              #+#    #+#             */
/*   Updated: 2025/08/11 00:32:55 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_len(t_data *data, t_token *token, int i, int *len)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	while (token->value[i] != '\0')
	{
		if (token->value[i] == '$')
		{
			i++;
			key = extract_key(&(token->value[i]));
			if (key[0] == '\0')
				*len += 1;
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			if (value)
				*len += ft_strlen(value);
			free(key);
		}
		else
		{
			(*len)++;
			i++;
		}
	}
}

static void	expand_with_variables(t_data *data, t_token *token, t_exp *node)
{
	int	len;

	len = 0;
	get_len(data, token, 0, &len);
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
		return ;
	expand_token_value(data, token, node, 0);
}

static void	set_exp_var(t_data *data, t_token *token, t_exp *new_node)
{
	if (token->type <= 9 && token->type >= 6)
		new_node->type = 5;
	else
		new_node->type = token->type;
	if (token->type == 9 || token->type == 8)
		expand_with_variables(data, token, new_node);
	else
	{
		new_node->exp_value = ft_strdup(token->value);
	}
	new_node->concat_w_next = token->concat_w_next;
	new_node->next = NULL;
}

static void	add_exp_node(t_data *data, t_token *token)
{
	t_exp	*new_node;
	t_exp	*tmp;

	tmp = NULL;
	new_node = malloc(sizeof(t_exp));
	if (!new_node)
		return ;
	set_exp_var(data, token, new_node);
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

	tmp = data->tokens;
	data->expander = NULL;
	while (tmp)
	{
		add_exp_node(data, tmp);
		tmp = tmp->next;
	}
    /*t_exp *debug = data->expander;
    while(debug)
    {
        printf("Expanded Value: %s TYPE: %d FLAG:%d\n", debug->exp_value, debug->type, debug->concat_w_next);
        debug = debug->next;
    }*/
	concatenator(data);
}
