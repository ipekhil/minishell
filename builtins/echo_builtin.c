#include "../minishell.h"

static int	is_n_flag(char *input)
{
	int	i;

	i = 0;
	if (!input || input[i] != '-')
		return (0);
	i++;
	while (input[i] == 'n')
		i++;
	if (input[i] != '\0')
		return (0);
	return (1);
}

int	echo_builtin(char **input)
{
	int i;
	int n_flag;

	i = 1;
	n_flag = 0;
	while (input[i] && is_n_flag(input[i]))
	{
		n_flag = 1;
		i++;
	}
	while (input[i])
	{
		printf("%s", input[i]);
		if (input[i + 1])
			printf(" ");
		i++;
	}
	if (n_flag == 0)
		printf("\n");
	return (0);
}
