/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:06:30 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/06 15:28:48 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	const unsigned char	*str;
	unsigned char		uc;

	str = ptr;
	uc = (unsigned char)c;
	while (n != 0)
	{
		n--;
		if (*str == uc)
			return ((void *)str);
		str++;
	}
	return (NULL);
}
