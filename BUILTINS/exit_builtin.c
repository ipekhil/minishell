/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:24:13 by staylan           #+#    #+#             */
/*   Updated: 2025/08/25 14:51:16 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_err(const char *msg, const char *arg, const char *msg2)
{
	write(2, msg, ft_strlen(msg));
	write(2, arg, ft_strlen(arg));
	write(2, msg2, ft_strlen(msg2));
}

static int	exit_with_args(t_data *data, char **args)
{
	int	i;

	i = 0;
	while (args[1][i])
	{
		if (args[1][i] == '-' || args[1][i] == '+')
			i++;
		if (!ft_isdigit(args[1][i]))
		{
			write_err("minishell: exit: ", args[1],
				": numeric argument required\n");
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
		write(2, "minishell: exit: too many arguments\n", 37);
		data->last_exit_status = 1;
		return ;
	}
	exit_code = ft_atoi(args[1]);
	if (data->last_exit_status != 2)
		data->last_exit_status = exit_code % 256;
	data->should_exit = 1;
	return ;
}
