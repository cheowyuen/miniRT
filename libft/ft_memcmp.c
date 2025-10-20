/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 09:26:11 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:11:51 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;
	size_t				i;

	if (n == 0)
		return (0);
	ptr1 = (const unsigned char *) s1;
	ptr2 = (const unsigned char *) s2;
	i = 0;
	while (i < (n - 1) && ptr1[i] == ptr2[i])
		i++;
	return (ptr1[i] - ptr2[i]);
}
