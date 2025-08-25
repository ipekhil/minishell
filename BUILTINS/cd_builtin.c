/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:47:56 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/25 14:54:48 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*new_node;

	env = is_there_var_in_env(env, key, value);
	if (!env)
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return ;
	}
	ft_lstadd_back(env, new_node);
}

static char	*search_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	if (!key)
		return (NULL);
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*get_cd_target(char **argv, t_env *env)
{
	char	*target;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		target = search_env_value(env, "HOME");
		if (!target)
		{
			write(2, "cd: HOME not set\n", 18);
			return (NULL);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		target = search_env_value(env, "OLDPWD");
		if (!target)
		{
			write(2, "cd: OLDPWD not set\n", 20);
			return (NULL);
		}
		printf("%s\n", target);
	}
	else
		target = argv[1];
	return (target);
}

static int	change_directory(char *target, t_env **env)
{
	char	*current_directory;
	char	*new_pwd;

	current_directory = getcwd(NULL, 0);
	if (!current_directory)
		return (1);
	if (chdir(target) == -1)
	{
		perror("minishell: cd");
		free(current_directory);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd");
		free(current_directory);
		return (1);
	}
	add_or_update_env(env, "PWD", new_pwd);
	add_or_update_env(env, "OLDPWD", current_directory);
	free(current_directory);
	free(new_pwd);
	return (0);
}

int	cd_builtin(t_data *data, char **argv)
{
	char	*target;
	int		argc;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc > 2)
	{
		write_err("minishell: ", argv[0], ": too many arguments\n");
		return (1);
	}
	target = get_cd_target(argv, data->env);
	if (!target)
		return (1);
	if (change_directory(target, &(data->env)))
		return (1);
	return (0);
}
