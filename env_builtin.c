#include "minishell.h"

void	env_builtin(t_env *env)
{
	while (env)
	{
		if (env->key)
			printf("%s=", env->key);
		if (env->value)
			printf("%s\n", env->value);
		env = env->next;
	}
}
