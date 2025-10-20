/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 08:26:23 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:20 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	length;

	length = 0;
	while (s[length])
		length++;
	while (length >= 0)
	{
		if (s[length] == (char) c)
			return ((char *)(s + length));
		length--;
	}
	return (NULL);
}
