/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:32:19 by sude              #+#    #+#             */
/*   Updated: 2025/07/24 00:05:08 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key(char *token_val)
{
	int		i;
	int		l;
	char	*key_to_search;

	i = 0;
	l = 0;
	while (token_val[l] && token_val[l] != '"' && !ft_isspace(token_val[l]))
		l++;
	key_to_search = malloc(sizeof(char) * (l + 1));
	while (i < l)
	{
		key_to_search[i] = token_val[i];
		i++;
	}
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

int	get_quote_len(t_data *data, int i)
{
	int		len;
	char	*key;
	char	*value;

	len = 0;
	while (data->tokens->value[i] != '"' && data->tokens->value[i] != '\0')
	{
		if (data->tokens->value[i] == '$')
		{
			i++;
			key = extract_key(&(data->tokens->value[i]));
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			if (value)
				len += ft_strlen(value);
			else
				value = "";
		}
		i++;
		len++;
	}
	return (len);
}

void	append_value(char *exp_value, char *value, int *a_index)
{
	int	i;

	i = 0;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}

void	double_quote_expand(t_data *data, int i)
{
	char	*key;
	char	*value;
	int		len;
	int		a_index;

	a_index = 0;
	value = NULL;
	key = NULL;
	len = get_quote_len(data, 1);
	data->expander->exp_value = malloc(sizeof(char) * (len + 1));
	while (data->tokens->value[i] != '"' && data->tokens->value[i] != '\0')
	{
		if (data->tokens->value[i] == '$')
		{
			i++;
			key = extract_key(&data->tokens->value[i]);
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			append_value(data->expander->exp_value, value, &a_index);
		}
		if (data->tokens->value[i] != '"')
			data->expander->exp_value[a_index++] = data->tokens->value[i];
		i++;
	}
	data->expander->exp_value[a_index] = '\0';
}
/*
void	single_quote_expand(t_data *data, int i)
{

}*/

void	expander(t_data *data)
{
	data->expander = malloc(sizeof(t_expander));
	while (data->tokens)
	{
		if (data->tokens->value[0] == '"')
			double_quote_expand(data, 1);
		if (!data->expander->exp_value)
			printf("yok\n");
		else
			printf("%s\n", data->expander->exp_value);
		/*if (data->tokens->value[0] == '\'')
			single_quote_expand(data, 1);*/
		data->tokens = data->tokens->next;
	}
}
