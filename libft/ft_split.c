/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:21:26 by cwong             #+#    #+#             */
/*   Updated: 2025/06/22 17:12:04 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strndup(const char *src, int n)
{
	int		i;
	char	*duplicate;

	duplicate = (char *) malloc ((n + 1) * sizeof(char));
	if (!duplicate)
		return (0);
	i = 0;
	while (i < n)
	{
		duplicate[i] = src[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}

static char	**ft_init_result(const char *s, char c)
{
	int		i;
	int		size;
	int		in_word;
	char	**result;

	if (!s)
		return (NULL);
	i = 0;
	size = 0;
	in_word = 0;
	while (s[i])
	{
		if (in_word == 0 && s[i] != c)
		{
			in_word = 1;
			size++;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	result = (char **) malloc((size + 1) * sizeof(char *));
	return (result);
}

static int	ft_word_len(const char *s, int *i, char c)
{
	int	len;

	while (s[*i] == c)
		(*i)++;
	len = 0;
	while (s[*i + len] && s[*i + len] != c)
		len++;
	return (len);
}

static char	**ft_free_all(char **result, int index)
{
	while (index > 0)
		free(result[--index]);
	free(result);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		index;
	char	**result;
	int		w_len;

	result = ft_init_result(s, c);
	if (!result)
		return (NULL);
	i = 0;
	index = 0;
	while (s[i])
	{
		w_len = ft_word_len(s, &i, c);
		if (w_len > 0)
		{
			result[index] = ft_strndup(&s[i], w_len);
			if (!result[index])
				return (ft_free_all(result, index));
			index++;
		}
		i += w_len;
	}
	result[index] = NULL;
	return (result);
}
