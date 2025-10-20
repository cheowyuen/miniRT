/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 08:19:40 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:22 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_in_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_start_index(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (ft_is_in_set(s1[i], set) == 1)
			i++;
		else
			break ;
	}
	return (i);
}

static int	ft_end_index(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i])
		i++;
	i--;
	while (i >= 0)
	{
		if (ft_is_in_set(s1[i], set) == 1)
			i--;
		else
			break ;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start_index;
	int		end_index;
	int		i;
	char	*ptr;

	if (!s1 || !set)
		return (NULL);
	start_index = ft_start_index(s1, set);
	end_index = ft_end_index(s1, set);
	if (start_index > end_index)
		return (ft_strdup(""));
	ptr = (char *) malloc(end_index - start_index + 2);
	if (!ptr)
		return (NULL);
	i = 0;
	while (start_index <= end_index)
	{
		ptr[i] = s1[start_index];
		i++;
		start_index++;
	}
	ptr[i] = '\0';
	return (ptr);
}
