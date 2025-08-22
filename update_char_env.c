/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_char_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:23:29 by ubuntu            #+#    #+#             */
/*   Updated: 2025/08/22 19:52:55 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*new_str;
	size_t	i;

	if (!s1 || !s2 || !s3)
		return (NULL);
	i = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	new_str = malloc(sizeof(char) * (i + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, i);
	ft_strlcat(new_str, s2, i);
	ft_strlcat(new_str, s3, i);
	return (new_str);
}

char	**convert_env_to_array(t_env *env)
{
	char	**env_array;
	t_env	*current;
	int		i;

	env_array = malloc(sizeof(char *) * (lstsize(env) + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			env_array[i] = ft_strjoin_three(current->key, "=", current->value);
			if (!env_array[i])
			{
				free_array(env_array);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
