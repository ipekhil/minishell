#include "minishell.h"

int	pwd_builtin(void)
{
	char *cwd;//current working directory

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
