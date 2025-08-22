/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:30:26 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 18:55:28 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		len;

	if (!dir || !cmd)
		return (NULL);
	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

static char	**search_in_env(char **env, char **paths)
{
	int		i;
	char	*path_env;

	path_env = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

static char *control_path_access(char **paths, char *cmd, char *full_path)
{
    int i;
    
    i = 0;
    while (paths[i]) 
    {
        full_path = create_full_path(paths[i], cmd);
        
        if (full_path) 
        {
            if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
            {
                free_array(paths);
                return (full_path);
            }
            else
                free(full_path);
        }
        i++;
    }
    free_array(paths);
    return (NULL);
}

char *find_relative_path(t_data *data, char *cmd, char **env)
{
    char **paths;
    char *full_path;
    struct stat st;

	full_path = NULL;
	paths = search_in_env(env, NULL);
	free_array(env);
	if (paths) 
	{
		full_path = control_path_access(paths, cmd, NULL);
		if (!full_path)
    		handle_err_and_exit(data, cmd, ": command not found\n", 127);
		if (stat(cmd, &st) == 0)
			pre_file_check(data, cmd, &data->last_exit_status);
		return (full_path);
	}
	if (stat(cmd, &st) == 0)
		pre_file_check(data, cmd, &data->last_exit_status);
	else
		handle_err_and_exit(data, cmd, ": No such file or directory\n", 127);
	return (NULL);
}

char *find_command_path(t_data *data, char *cmd, char **env)
{
    char *full_path;
    
	full_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		free_array(env);
		pre_file_check(data, cmd, &data->last_exit_status);
		return (ft_strdup(cmd));
    }
	full_path = find_relative_path(data, cmd, env);
	return (full_path);
}
