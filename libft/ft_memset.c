/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:31:15 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/11 20:41:34 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char	*a;

	a = (char *)b;
	while (len != 0)
	{
		len--;
		a[len] = c;
	}
	return ((void *)b);
}
#include <stdio.h>
int main()
{
	int str[] = {1,2,3,4,5};
	ft_memset(str + 4,255,4);
	ft_memset(str + 4, 254, 1);
	printf("%d", str[4]);
}