/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 01:20:10 by sude              #+#    #+#             */
/*   Updated: 2025/08/22 19:38:53 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_env_value(t_env *env, char *env_var, int *j)
{
	int	len;
	int	start;

	len = 0;
	start = ++(*j);
	while (env_var[start] && env_var[start] != '\0')
	{
		start++;
		len++;
	}
	env->value = malloc(sizeof(char) * (len + 1));
	if (!env->value)
		return (-1);
	len = 0;
	while (env_var[*j] && env_var[*j] != '\0')
	{
		env->value[len++] = env_var[*j];
		(*j)++;
	}
	env->value[len] = '\0';
	return (0);
}

static int	get_env_key(t_env *env, char *env_var, int *j)
{
	int	len;
	int	start;

	len = 0;
	start = *j;
	while (env_var[*j] && env_var[*j] != '=')
	{
		len++;
		(*j)++;
	}
	env->key = malloc(sizeof(char) * (len + 1));
	if (!env->key)
		return (-1);
	len = 0;
	while (env_var[start] && env_var[start] != '=')
	{
		env->key[len++] = env_var[start++];
	}
	env->key[len] = '\0';
	return (0);
}

void	ft_lstadd_back(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

void	get_env(t_env **env, char **envp)
{
	int		i;
	int		j;
	t_env	*new_node;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = 0;
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->next = NULL;
		if (get_env_key(new_node, envp[i], &j) == -1)
			return ;
		if (envp[i][j] == '=')
			if (get_env_value(new_node, envp[i], &j) == -1)
				return ;
		ft_lstadd_back(env, new_node);
		i++;
	}
}
