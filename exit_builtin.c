/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:27:41 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/17 17:34:16 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_with_args(t_data *data, char **args)
{
	int	i;

	i = 0;
	while (args[1][i])
	{
		if (args[1][i] == '-' || args[1][i] == '+')
			i++;
		if (!ft_isdigit(args[1][i]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			data->last_exit_status = 2;
			data->should_exit = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

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
	printf("exit\n");
	i = 0;
	if (args[1][i] && !exit_with_args(data, args))
		return ;
	if (args[2] && exit_with_args(data, args))
	{
		printf("minishell: exit: too many arguments\n");
		data->last_exit_status = 1;
		return ;
	}
	exit_code = ft_atoi(args[1]);
	if (data->last_exit_status != 2)
		data->last_exit_status = exit_code % 256;
	data->should_exit = 1;
	return ;
}
