#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	if (!key)
		return (NULL);
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*get_cd_target(char **argv, t_env *env)
{
	char	*target;

	if (!argv[1])
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (!target)
		{
			printf("cd: OLDPWD not set\n");
			return (NULL);			
		}
		printf("%s\n", target);
	}
	else
		target = argv[1];
	return (target);
}

static int	change_directory(char *target, t_env **env)
{
	char	*current_directory;
	char	*new_pwd;

	current_directory = getcwd(NULL, 0);
	if (!current_directory)
		return (1);
	if (chdir(target) == -1)
	{
		perror("cd");
		free(current_directory);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd");
		free(current_directory);
		return (1);
	}
	add_or_update_env(env, "PWD", new_pwd);
	add_or_update_env(env, "OLDPWD", current_directory);
	free(current_directory);
	free(new_pwd);
	return (0);
}

int	cd_builtin(t_data *data, char **argv)
{
	char	*target;

	if (argv[2])
	{
		printf("minishell: %s: too many arguments\n", argv[0]);
		return (1);
	}
	target = get_cd_target(argv, data->env);
	if (!target)
		return (1);
	if (change_directory(target, &(data->env)))
		return (1);
	return (0);
}
