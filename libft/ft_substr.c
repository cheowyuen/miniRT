/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:15:56 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:23 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	nmemb;
	size_t	s_len;
	char	*ptr;

	if (!s)
		return (NULL);
	nmemb = len;
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if ((start + len) > s_len)
		nmemb = s_len - start;
	ptr = (char *) malloc(nmemb + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	j = start;
	while (s[j] && i < len)
		ptr[i++] = s[j++];
	ptr[i] = '\0';
	return (ptr);
}
