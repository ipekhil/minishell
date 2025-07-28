/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:13 by sude              #+#    #+#             */
/*   Updated: 2025/07/28 15:35:47 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
	{
		i++;
	}
	return (s1[i] - s2[i]);
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

char	*ft_strdup(const char *s1)
{
	int		size;
	int		i;
	char	*ptr;

	i = -1;
	size = ft_strlen(s1);
	ptr = (char *) malloc(size + 1);
	if (ptr == NULL)
		return (NULL);
	while (++i < size)
		ptr[i] = s1[i];
	ptr[i] = '\0';
	return (ptr);
}

void	ft_bzero(void *s, size_t n)
{
	char	*a;

	a = (char *)s;
	while (n != 0)
	{
		n--;
		a[n] = '\0';
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*s;
	size_t	n;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	n = count * size;
	s = malloc(n);
	if (s == NULL)
		return (NULL);
	else
		ft_bzero(s, n);
	return (s);
}