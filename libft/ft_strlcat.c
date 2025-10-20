/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:04:45 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:12 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t		src_length;
	size_t		dest_length;
	size_t		i;
	size_t		length;

	dest_length = 0;
	while (dst[dest_length])
		dest_length++;
	src_length = 0;
	while (src[src_length])
		src_length++;
	if (dest_length >= size)
		length = src_length + size;
	else
		length = src_length + dest_length;
	i = 0;
	while (src[i] && (dest_length + 1 < size))
	{
		dst[dest_length] = src[i];
		dest_length++;
		i++;
	}
	dst[dest_length] = '\0';
	return (length);
}
