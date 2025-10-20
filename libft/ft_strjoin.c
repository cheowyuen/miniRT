/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:56:35 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:09 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	ptr_len;

	if (!s1 || !s2)
		return (NULL);
	ptr_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *) malloc(ptr_len);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, ptr_len);
	ft_strlcat(ptr, s2, ptr_len);
	return (ptr);
}
