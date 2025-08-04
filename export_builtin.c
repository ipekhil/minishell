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
			if (value && !tmp->value)
			{
				tmp->value = ft_strdup(value);
				free(tmp->value);
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

	if (!*key)
		return (0);
	i = 0;
	while (key[i])
	{
		if (!((key[i] >= 'a' && key[i] <= 'z') || \
		(key[i] >= 'A' && key[i] <= 'Z') || \
		(key[i] >= '0' && key[i] <= '9') || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	export_builtin(t_data *data, char **args)
{
	char	**current_arg;
	char	*equal_sign;
	char	*key;
	char	*value;

	current_arg = &args[1];
	if (!*current_arg)
	{
		print_export_env(data);
		return ;
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
			{
				perror("export_builtin");
				return ;
			}
			value = ft_substr(equal_sign + 1, 0, ft_strlen(equal_sign + 1));
			if (!value)
			{
				perror("export_builtin");
				return ;
			}
			if (!is_valid_key(key))
			{
				printf("export: `%s': not a valid identifier\n", *current_arg);
				return ;
			}
			add_or_update_env(&data->env, key, value);
			free(key);
			free(value);
		}
		else
		{
			key = ft_strdup(*current_arg);
			if (!key)
			{
				perror("export_builtin");
				return ;
			}
			if (!is_valid_key(key))
			{
				printf("export: `%s': not a valid identifier\n", *current_arg);
				return ;
			}
			add_or_update_env(&data->env, key, NULL);
			free(key);
		}
		current_arg++;
	}
}
