/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 08:57:52 by cwong             #+#    #+#             */
/*   Updated: 2025/06/28 18:12:54 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(int fd, char *str, char c)
{
	int	len;

	if (c == 'c' && str[0] == '\0')
	{
		if (write(fd, "\0", 1) == -1)
			return (-1);
		return (1);
	}
	len = ft_strlen(str);
	if (write(fd, str, len) == -1)
		return (-1);
	return (len);
}

static int	ft_len(unsigned int nbr)
{
	int	len;

	len = 1;
	while (nbr >= 10)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char	*ft_unsigned_itoa(unsigned int nbr)
{
	char	*ptr;
	int		len;

	len = ft_len(nbr);
	ptr = (char *) malloc(len + 1);
	if (!ptr)
		return (NULL);
	ptr[len--] = '\0';
	while (len >= 0)
	{
		ptr[len] = (nbr % 10) + '0';
		nbr = nbr / 10;
		len--;
	}
	return (ptr);
}

char	*process_string(const char *str)
{
	if (!str)
		return ("(null)");
	return ((char *) str);
}

void	convert_to_hex(uintptr_t nbr, int is_lower_case,
	int has_prefix, char *hex)
{
	char	reversed_hex[20];
	char	*base;
	int		i;
	int		j;

	if (is_lower_case)
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	i = 0;
	while (nbr != 0)
	{
		reversed_hex[i++] = base[nbr % 16];
		nbr /= 16;
	}
	if (has_prefix)
	{
		reversed_hex[i++] = 'x';
		reversed_hex[i++] = '0';
	}
	i--;
	j = 0;
	while (i >= 0)
		hex[j++] = reversed_hex[i--];
	hex[j] = '\0';
}
