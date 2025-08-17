#include "minishell.h"

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new_node;

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
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		perror("add_or_update_env");
		return ;
	}
	ft_lstadd_back(env, new_node);
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

int	is_valid_key(char *key)
{
	int	i;

	if (!key)
		return (0);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	export_builtin(t_data *data, char **args)
{
	char	**current_arg;
	char	*equal_sign;
	char	*key;
	char	*value;

	data->last_exit_status = 0;
	current_arg = &args[1];
	if (!*current_arg)
	{
		print_export_env(data);
		return (0);
	}
	key = NULL;
	value = NULL;
	while (*current_arg)
	{
		equal_sign = ft_strchr(*current_arg, '=');
		if (equal_sign)
		{
			key = ft_substr(*current_arg, 0, equal_sign - *current_arg);
			if (!key)
				return (1);
			value = ft_substr(equal_sign + 1, 0, ft_strlen(equal_sign + 1));
			if (!value)
				return (1);
			if (is_valid_key(key))
			{
				add_or_update_env(&data->env, key, value);
				free(key);
				free(value);
			}
			else
			{
				printf("export: `%s': not a valid identifier\n", *current_arg);
				free(key);
				free(value);
				data->last_exit_status = 1;
			}
		}
		else
		{
			key = ft_strdup(*current_arg);
			if (is_valid_key(key))
			{
				add_or_update_env(&data->env, key, NULL);
				free(key);
			}
			else
			{
				printf("export: `%s': not a valid identifier\n", *current_arg);
				free(key);
				data->last_exit_status = 1;
			}
		}
		current_arg++;
	}
	return (data->last_exit_status);
}
