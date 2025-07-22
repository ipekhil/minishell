/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:13 by sude              #+#    #+#             */
/*   Updated: 2025/07/22 00:21:14 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int i;

	i = 0;
	while(src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
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
char **tokens_to_args(t_token *tokens, int count)
{
	char **args;
	int i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		args[i] = tokens[i].value;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	execute_builtin(t_data	*data)
{
	char	**args;

	args = tokens_to_args(data->tokens, data->token_count);
	if (!args)
		return ;
	if (ft_strcmp(args[0], "pwd") == 0)
		pwd_builtin();
	else if (ft_strcmp(args[0], "cd") == 0)
		cd_builtin(args, &data->env);
	else if (ft_strcmp(args[0], "echo") == 0)
		echo_builtin(args);
/**
	else if (strcmp(data->tokens[0], "exit") == 0)
		builtin_exit();
	else if (strcmp(data->tokens[0], "env") == 0)
		builtin_env(data->env);
 */
	free(args);
	return ;
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_isspace(char c)
{
	if ((c <= 12 && c >= 9) || c == 32)
		return (1);
	return (0);
}

int	ft_isoperator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srcsize;

	i = 0;
	srcsize = ft_strlen(src);
	if (!src)
		return (0);
	if (!dst)
		return (srcsize);
	if (dstsize > 0)
	{
		while (i < dstsize - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srcsize);
}

