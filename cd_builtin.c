#include "minishell.h"

static char	*get_cd_target(char **argv)
{
	char	*target;

	if (!argv[1])
	{
		target = getenv("HOME");
		if (!target)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		target = getenv("OLDPWD");
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

static int	change_directory(char *target, char ***envp)
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
	update_env_var(envp, "PWD", new_pwd);
	update_env_var(envp, "OLDPWD", current_directory);
	free(current_directory);
	free(new_pwd);
	return (0);
}

void	cd_builtin(char **argv, char ***envp)
{
	char	*target;

	target = get_cd_target(argv);
	if (!target)
		return ;
	if (change_directory(target, envp))
		return ;
}
