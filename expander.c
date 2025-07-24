/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:32:19 by sude              #+#    #+#             */
/*   Updated: 2025/07/26 00:03:35 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key(char *token_val)
{
	int		l;
	int		i;
	char	*key_to_search;

	l = 0;
	i = -1;
	while (token_val[l] != '\'' && token_val[l]
		&& token_val[l] != '"' && !ft_isspace(token_val[l]))
		l++;
	key_to_search = malloc(sizeof(char) * (l + 1));
	if (!key_to_search)
		return (NULL);
	while (++i < l)
		key_to_search[i] = token_val[i];
	key_to_search[l] = '\0';
	return (key_to_search);
}

char	*get_value_of_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	get_quote_len(t_data *data, t_token *token, int i)
{
	int		len;
	char	*key;
	char	*value;


	len = 0;
	key = NULL;
	value = NULL;
	while (token->value[i] != '"' && token->value[i] != '\0')
	{
		if (token->value[i] == '$')
		{
			i++;
			key = extract_key(&(token->value[i]));
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			if (value)
				len += ft_strlen(value);
			free(key);
		}
		i++;
		len++;
	}
	return (len);
}

void	append_value(char *exp_value, char *value, int *a_index)
{
	int i = 0;
	if (!value)
		return;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}

void	double_quote_expand(t_data *data, t_token *token, t_expander *node, int i)
{
	char	*key;
	char	*value;
	int		len;
	int		a_index;

	key = NULL;
	value = NULL;
	a_index = 0;
	len = get_quote_len(data, token, i);
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
		return;
	while (token->value[i] != '"' && token->value[i] != '\0')
	{
		if (token->value[i] == '$')
		{
			i++;
			key = extract_key(&token->value[i]);
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			append_value(node->exp_value, value, &a_index);
			free(key);
		}
		if (data->tokens->value[i] != '"')
			node->exp_value[a_index++] = token->value[i];
		i++;
	}
	node->exp_value[a_index] = '\0';
}

void	single_quote_expand(t_token *token, t_expander *node, int i)
{
	int	len;
	int	k;

	len = 0;
	k = 0;
	while (token->value[i + len] != '\'' && token->value[i + len] != '\0')
		len++;
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
		return;
	while (k < len)
		node->exp_value[k++] = token->value[i++];
	node->exp_value[k] = '\0';
}

void	expand_unquoted(t_data *data, t_token *token, t_expander *node)
{
	char	*key;
	char	*value;
	int		len;
	int		k;

	k = -1;
	key = extract_key(&(token->value[1]));
	value = get_value_of_key(data->env, key);
	if (!value)
		value = "";
	len = ft_strlen(value);
	node->exp_value = malloc(sizeof(char) * (len + 1));
	if (!node->exp_value)
	{
		free(key);
		return;
	}
	while (value[++k] != '\0')
		node->exp_value[k] = value[k];
	node->exp_value[k] = '\0';
	free(key);
}

void    expander(t_data *data)
{
    t_expander *new_node;
    t_expander *last;
    t_token *tmp;

	tmp = data->tokens;
    data->expander = NULL;
    while (tmp)
    {
        new_node = malloc(sizeof(t_expander));
        if (!new_node)
            return ;
        new_node->exp_value = NULL;
        new_node->next = NULL;
        if (tmp->value[0] == '"')
            double_quote_expand(data, tmp, new_node, 1);
        else if (tmp->value[0] == '\'')
            single_quote_expand(tmp, new_node, 1);
        else if (tmp->value[0] == '$')
            expand_unquoted(data, tmp, new_node);
        else
            new_node->exp_value = strdup(tmp->value);
        if (!data->expander)
            data->expander = new_node;
        else
        {
            last = data->expander;
            while (last->next)
                last = last->next;
            last->next = new_node;
        }
		new_node->type = tmp->type;
        tmp = tmp->next;
    }
	/*while(data->expander)
	{
		printf("Expanded Value: %s\n", data->expander->exp_value);
		data->expander = data->expander->next;
	}*/
	parser(data);
}
