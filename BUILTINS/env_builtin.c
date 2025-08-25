/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:50:32 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/25 14:51:16 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			return (NULL);
		}
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	env_builtin(t_data *data, char **args)
{
	t_env	*env;

	if (args[1])
	{
		write_err("minishell: env: ", args[1], ": No such file or directory\n");
		data->last_exit_status = 127;
		return (127);
	}
	env = data->env;
	while (env && env->value)
	{
		if (env->key)
			printf("%s=", env->key);
		if (env->value)
			printf("%s\n", env->value);
		env = env->next;
	}
	data->last_exit_status = 0;
	return (0);
}
