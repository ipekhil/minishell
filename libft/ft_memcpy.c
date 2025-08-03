/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:22:32 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/06 15:27:09 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	int			i;
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	i = 0;
	if (!dest && !src)
		return (0);
	while (n != 0)
	{
		n--;
		d[i] = s[i];
		i++;
	}
	return (dest);
}
