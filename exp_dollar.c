/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:49:52 by sude              #+#    #+#             */
/*   Updated: 2025/08/22 19:11:47 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_exp_len(t_data *data, char *first_val, int *i, int *len)
{
	char	*key;
	char	*value;
	int		app_flag;

	key = NULL;
	value = NULL;
	app_flag = 0;
	(*i)++;
	key = extract_key(&(first_val[*i]));
	if (key[0] == '\0')
		*len += 1;
	*i += ft_strlen(key);
	value = get_value_of_key(data, key, &app_flag);
	if (value)
		*len += ft_strlen(value);
	free(key);
	if (app_flag)
		free(value);
}

void	get_len(t_data *data, char *first_val, int i, int *len)
{
	while (first_val[i] != '\0')
	{
		if (first_val[i] == '$')
			get_exp_len(data, first_val, &i, len);
		else
		{
			(*len)++;
			i++;
		}
	}
}

static void	handle_dollar_expansion(t_data *data, char *first_val, int *i, char *new_val, int *a_index)
{
	char	*key;
	char	*value;
	int		append_flag;

	append_flag = 0;
	(*i)++;
	key = extract_key(&first_val[*i]);
	if (key[0] == '\0')
		new_val[(*a_index)++] = '$';
	*i += ft_strlen(key);
	value = get_value_of_key(data, key, &append_flag);
	append_value(new_val, value, a_index);
	free(key);
	if (append_flag)
		free(value);
}

void	expand_token_value(t_data *data, char *first_val, char *new_val, int i)
{
	int	a_index;

	a_index = 0;
	while (first_val[i] != '\0')
	{
		if (first_val[i] == '$')
			handle_dollar_expansion(data, first_val, &i, new_val, &a_index);
		else
			new_val[a_index++] = first_val[i++];
	}
	new_val[a_index] = '\0';
}
