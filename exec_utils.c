/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:02:16 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 19:01:52 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_was_interrupted(t_parser *cmds)
{
	while (cmds)
	{
		if (cmds->redirection && cmds->redirection->hdoc_int == 1)
			return (1);
		cmds = cmds->next;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_data *data, char **args)
{
	if (!args)
		return ;
	if (ft_strcmp(args[0], "pwd") == 0)
		data->last_exit_status = pwd_builtin();
	else if (ft_strcmp(args[0], "cd") == 0)
		data->last_exit_status = cd_builtin(data, args);
	else if (ft_strcmp(args[0], "echo") == 0)
		data->last_exit_status = echo_builtin(args);
	else if (ft_strcmp(args[0], "export") == 0)
		export_builtin(data, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset_builtin(data, args);
	else if (ft_strcmp(args[0], "env") == 0)
		env_builtin(data, args);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_builtin(data, args);
	return ;
}

int	lstsize(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}
