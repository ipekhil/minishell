/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:20:34 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 19:11:13 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	**is_there_var_in_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				if (value)
					tmp->value = ft_strdup(value);
				else
					tmp->value = NULL;
			}
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (env);
}

static void	print_export_env(t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (tmp->key)
		{
			printf("declare -x %s", tmp->key);
			if (tmp->value)
				printf("=\"%s\"", tmp->value);
			printf("\n");
		}
		tmp = tmp->next;
	}
}

static int	handle_key_value(t_data *data, char *current_arg, char *equal_sign)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (equal_sign)
	{
		key = ft_substr(current_arg, 0, equal_sign - current_arg);
		if (!key)
			return (free(key), 1);
		value = ft_substr(equal_sign + 1, 0, ft_strlen(equal_sign + 1));
		if (!value)
			return (free(key), free(value), 1);
		if (is_valid_key(key))
			add_or_update_env(&data->env, key, value);
		else
		{
			write_err("export: `", current_arg, "': not a valid identifier\n");
			data->last_exit_status = 1;
			return (free(key), free(value), 1);
		}
		free(key);
		free(value);
	}
	return (0);
}

void	handle_only_key(t_data *data, char *current_arg, char *key)
{
	key = ft_strdup(current_arg);
	if (is_valid_key(key))
	{
		add_or_update_env(&data->env, key, NULL);
		free(key);
	}
	else
	{
		write_err("export: `", current_arg, "': not a valid identifier\n");
		free(key);
		data->last_exit_status = 1;
	}
}

int	export_builtin(t_data *data, char **args)
{
	char	**current_arg;
	char	*equal_sign;
	char	*key;

	key = NULL;
	data->last_exit_status = 0;
	current_arg = &args[1];
	if (!*current_arg)
	{
		print_export_env(data);
		return (0);
	}
	while (*current_arg)
	{
		equal_sign = ft_strchr(*current_arg, '=');
		if (handle_key_value(data, *current_arg, equal_sign) == 0
			&& !equal_sign)
			handle_only_key(data, *current_arg, key);
		current_arg++;
	}
	return (data->last_exit_status);
}
