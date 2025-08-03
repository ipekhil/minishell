#include "minishell.h"

void	pwd_builtin(void)
{
	char *cwd;//current working directory

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return;
	}
	printf("%s\n", cwd);
	free(cwd);
}
