/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:24:24 by staylan           #+#    #+#             */
/*   Updated: 2025/08/25 14:51:16 by staylan          ###   ########.fr       */
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
		&& token_val[l] != '$' && token_val[l] != '/'
		&& token_val[l] != ':')
		l++;
	key_to_search = malloc(sizeof(char) * (l + 1));
	if (!key_to_search)
		return (NULL);
	while (++i < l)
		key_to_search[i] = token_val[i];
	key_to_search[l] = '\0';
	return (key_to_search);
}

static char	*expand_exit_status(t_data *data, char *key, int *app_fl)
{
	char	*val;
	char	*new_v;
	int		len;

	val = ft_itoa(data->last_exit_status);
	len = ft_strlen(val);
	new_v = NULL;
	if (key[1] != '\0')
		len += ft_strlen(&key[1]);
	new_v = malloc(len + 1);
	if (!new_v)
	{
		free(val);
		return (NULL);
	}
	ft_strcpy(new_v, val);
	if (key[1] != '\0')
		ft_strlcat(new_v, &key[1], len + 1);
	*app_fl = 1;
	free(val);
	return (new_v);
}

char	*get_value_of_key(t_data *data, char *key, int *app_fl)
{
	t_env	*env;
	char	*new_v;

	env = data->env;
	new_v = NULL;
	if (key[0] == '?')
	{
		new_v = expand_exit_status(data, key, app_fl);
		return (new_v);
	}
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	append_value(char *exp_value, char *value, int *a_index)
{
	int	i;

	i = 0;
	if (!value)
		return ;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}
