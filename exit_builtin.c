#include "minishell.h"

void	exit_builtin(t_data *data, char **args)
{
	long long	exit_code;
	int			i;

	if (!args[1])
	{
		data->should_exit = 1;
		printf("exit\n");
		return ;
	}
	i = 0;
	while (args[1][i])
	{
		if (args[1][i] == '-' || args[1][i] == '+')
			i++;
		if (!ft_isdigit(args[1][i]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			data->last_exit_status = 255;
			data->should_exit = 1;
			return ;
		}
		i++;
	}
	if (args[2])
	{
		printf("minishell: exit: too many arguments\n");
		return ;
	}
	exit_code = ft_atoi(args[1]);
	data->last_exit_status = exit_code % 256;
	data->should_exit = 1;
	printf("exit\n");
	return ;
}
