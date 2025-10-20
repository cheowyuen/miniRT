/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 07:23:26 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:11:36 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long int	ft_len(long int n)
{
	long int	len;

	len = 1;
	while (n >= 10)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	ft_init_itoa(int n, long int *nbr, long int *len)
{
	if (n < 0)
	{
		*nbr = ((long int)(n)) * -1;
		*len = ft_len(*nbr) + 1;
	}
	else
	{
		*nbr = (long int) n;
		*len = ft_len(*nbr);
	}
}

char	*ft_itoa(int n)
{
	int			i;
	char		*ptr;
	long int	len;
	long int	nbr;

	ft_init_itoa(n, &nbr, &len);
	ptr = (char *) malloc(len + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	if (n < 0)
	{
		ptr[i] = '-';
		i++;
	}
	ptr[len] = '\0';
	len--;
	while (len >= i)
	{
		ptr[len] = (nbr % 10) + '0';
		nbr = nbr / 10;
		len--;
	}
	return (ptr);
}
