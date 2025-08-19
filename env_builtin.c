/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:50:32 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/19 03:38:10 by ubuntu           ###   ########.fr       */
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

static char	*join_env_var(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*new_var;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
		return (NULL);
	ft_strcpy(new_var, name);
	new_var[name_len] = '=';
	ft_strcpy(new_var + name_len + 1, value);
	return (new_var);
}

static int	add_env_var(char ***envp, const char *name, const char *value)
{
	char	**new_envp;
	char	*new_var;
	int		i;
	int		j;

	i = 0;
	while ((*envp)[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (1);
	j = -1;
	while (++j < i)
		new_envp[j] = (*envp)[j];
	new_var = join_env_var(name, value);
	if (!new_var)
		return (1);
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

void	update_env_var(char ***envp, const char *name, const char *value)
{
	size_t	name_len;
	char	*new_var;
	int		i;

	if (!envp || !name || !value)
		return ;
	name_len = ft_strlen(name);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, name_len) == 0 \
			&& (*envp)[i][name_len] == '=')
		{
			new_var = join_env_var(name, value);
			if (!new_var)
				return ;
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return ;
		}
		i++;
	}
	if (add_env_var(envp, name, value))
		return ;
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
