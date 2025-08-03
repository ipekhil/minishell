/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:22:14 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/11 18:13:05 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_wordcount(char const *s, char c)
{
	int	wordcount;
	int	flagstart;

	wordcount = 0;
	flagstart = 0;
	while (*s)
	{
		if (*s == c)
			flagstart = 0;
		else if (flagstart == 0)
		{
			flagstart = 1;
			wordcount++;
		}
		s++;
	}
	return (wordcount);
}

char	*ft_getword(const char *s, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((len + 1) * (sizeof(char)));
	if (word == NULL)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_fill(char **result, char const *s, char c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[j++] = ft_getword(&s[start], i - start);
		}
		else
			i++;
	}
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;

	if (s == NULL)
		return (NULL);
	count = ft_wordcount(s, c);
	result = malloc((count + 1) * (sizeof(char *)));
	if (result == NULL)
		return (NULL);
	ft_fill(result, s, c);
	result[count] = NULL;
	return (result);
}
