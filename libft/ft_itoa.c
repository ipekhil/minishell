/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:21 by hiipek            #+#    #+#             */
/*   Updated: 2024/11/11 14:18:18 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lenln(int n, int sign)
{
	int	len;

	len = 0;
	if (n == 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	if (sign < 0)
		len++;
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	ln;
	int		sign;
	int		lenstr;

	ln = n;
	sign = 1;
	if (ln < 0)
		sign = -1;
	ln = sign * ln;
	lenstr = ft_lenln(ln, sign);
	str = (char *)malloc((lenstr + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[lenstr] = '\0';
	while (--lenstr >= 0)
	{
		str[lenstr] = (ln % 10) + '0';
		ln = ln / 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}
