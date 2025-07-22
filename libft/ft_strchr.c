/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:58:12 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/11 14:31:46 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*str != '\0')
	{
		if (*str == uc)
			return ((char *)str);
		str++;
	}
	if (uc == '\0' && *str == '\0')
		return ((char *)str);
	return (NULL);
}
