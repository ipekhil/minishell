/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:14:38 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/06 14:59:51 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*result;

	while (*s1)
	{
		if (ft_strchr(set, *s1) == 0)
			break ;
		s1++;
	}
	len = ft_strlen(s1);
	while (len && s1[len - 1])
	{
		if (ft_strchr(set, s1[len - 1]) == 0)
			break ;
		len--;
	}
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (0);
	ft_strlcpy(result, s1, len + 1);
	return (result);
}
