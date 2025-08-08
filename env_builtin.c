#include "minishell.h"

int	env_builtin(t_data *data, char **args)
{
	t_env	*env;

	if (args[1])
	{
		printf("minishell: env: %s: No such file or directory", args[1]);
		data->last_exit_status = 127;
		return (127);
	}
	env = data->env;
	while (env && env->key)
	{
		if (env->key)
			printf("%s=", env->key);
		if (env->value)
			printf("%s\n", env->value);
		env = env->next;
	}
	data->last_exit_status = 0;
	return (0);
}
