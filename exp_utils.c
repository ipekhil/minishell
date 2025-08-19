/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:09:00 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/19 19:09:39 by ubuntu           ###   ########.fr       */
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
	while (token_val[l] && !ft_isspace(token_val[l])
		&& token_val[l] != '"' && token_val[l] != '\''
		&& token_val[l] != '$')
		l++;
	key_to_search = malloc(sizeof(char) * (l + 1));
	if (!key_to_search)
		return (NULL);
	while (++i < l)
		key_to_search[i] = token_val[i];
	key_to_search[l] = '\0';
	return (key_to_search);
}

char	*get_value_of_key(t_data *data, char *key, int *app_fl)
{
	t_env	*env;
	char	*val;
	int		len;
	char	*new_v;

	env = data->env;
	val = NULL;
	new_v = NULL;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		if (key[0] == '?')
		{
			val = ft_itoa(data->last_exit_status);
			len = ft_strlen(val);
			if (key[1] != '\0')
				len += ft_strlen(&key[1]);
			new_v = malloc(len + 1);
			ft_strcpy(new_v, val);
			ft_strlcat(new_v, &key[1], len + 1);
			*app_fl = 1;
			free(val);
			return (new_v);
		}
		env = env->next;
	}
	return (NULL);
}

static void	append_value(char *exp_value, char *value, int *a_index)
{
	int	i;

	i = 0;
	if (!value)
		return ;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}

void	expand_token_value(t_data *data, char *first_val, char *new_val, int i)
{
	char	*key;
	char	*value;
	int		a_index;
	int		append_flag;

	a_index = 0;
	key = NULL;
	value = NULL;
	append_flag = 0;
	while (first_val[i] != '\0')
	{
		append_flag = 0;
		if (first_val[i] == '$')
		{
			i++;
			key = extract_key(&first_val[i]);
			if (key[0] == '\0')
				new_val[a_index++] = '$';
			i += ft_strlen(key);
			value = get_value_of_key(data, key, &append_flag);
			append_value(new_val, value, &a_index);
			free(key);
			if (append_flag)
				free(value);
		}
		else
			new_val[a_index++] = first_val[i++];
	}
	new_val[a_index] = '\0';
}
