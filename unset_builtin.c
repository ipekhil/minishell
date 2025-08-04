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

void	unset_builtin(t_data *data, char **args)
{
	char	**current_arg;

	current_arg = &args[1];
	while (*current_arg)
	{
		remove_env(&data->env, *current_arg);
		current_arg++;
	}
}