/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:32:19 by sude              #+#    #+#             */
/*   Updated: 2025/08/16 00:52:14 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_len(t_data *data, char *first_val, int i, int *len)
{
	char	*key;
	char	*value;
	int		app_flag;
	

	key = NULL;
	value = NULL;
	app_flag = 0;
	while (first_val[i] != '\0')
	{
		if (first_val[i] == '$')
		{
			i++;
			key = extract_key(&(first_val[i]));
			if (key[0] == '\0')
				*len += 1;
			i += ft_strlen(key);
			value = get_value_of_key(data, key, &app_flag);
			if (value)
				*len += ft_strlen(value);	
			free(key);
			if(app_flag)
				free(value);
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
	get_len(data, token->value, 0, &len);
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
		return ;
	expand_token_value(data, token->value, node->exp_value, 0);
}

static void	set_exp_var(t_data *data, t_token *token, t_exp *new_node, int exp_fl)
{
	if (token->type <= 9 && token->type >= 6)
		new_node->type = 5;
	else
		new_node->type = token->type;
	if ((token->type == 9 || token->type == 8) && exp_fl)
		expand_with_variables(data, token, new_node);
	else
	{
		new_node->exp_value = ft_strdup(token->value);
	}
	new_node->concat_w_next = token->concat_w_next;
	new_node->next = NULL;
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

	tmp = data->tokens;
	data->expander = NULL;
	expand_flag = 1;
	while (tmp)
	{
		int len = 0;
		get_len(data, tmp->value, 0, &len);
		if (len != 0)
		{
			add_exp_node(data, tmp, expand_flag);
			expand_flag = 1;
			if (tmp->type == 0 && tmp->next)
				expand_flag = 0;	
		}
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
