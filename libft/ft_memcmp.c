/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:51:21 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/06 17:01:06 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *ptr1, const void *ptr2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;

	p1 = (const unsigned char *)ptr1;
	p2 = (const unsigned char *)ptr2;
	while (n > 0)
	{
		if (*p1 != *p2)
			return ((int)*p1 - *p2);
		p1++;
		p2++;
		n--;
	}
	return (0);
}
