#include "minishell.h"

void	remove_env(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->key);
			if (tmp->value)
				free(tmp->value);
			if (!prev)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	unset_builtin(t_data *data, char **args)
{
	char	**current_arg;

	data->last_exit_status = 0;
	current_arg = &args[1];
	while (*current_arg)
	{
		if (!is_valid_key(*current_arg))
		{
			printf("unset: `%s': not a valid identifier\n", *current_arg);
			data->last_exit_status = 1;
		}
		else
			remove_env(&data->env, *current_arg);
		current_arg++;
	}
	return (data->last_exit_status);
}
