/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:41:14 by cwong             #+#    #+#             */
/*   Updated: 2025/07/01 07:20:59 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*int_to_hex(uintptr_t nbr, int is_lower_case, int has_prefix)
{
	char	*hex;

	if (nbr == 0)
	{
		if (has_prefix)
			return (ft_strdup("(nil)"));
		else
			return (ft_strdup("0"));
	}
	hex = malloc(20);
	if (!hex)
		return (NULL);
	convert_to_hex(nbr, is_lower_case, has_prefix, hex);
	return (hex);
}

static char	*handle_percent(const char *str, va_list args, char *c)
{
	if (*str == 'd' || *str == 'i')
		return (ft_itoa(va_arg(args, int)));
	else if (*str == 's')
		return (process_string(va_arg(args, char *)));
	else if (*str == 'c')
	{
		c[0] = (char) va_arg(args, int);
		c[1] = '\0';
		return (c);
	}
	else if (*str == '%')
		return ("%");
	else if (*str == 'p')
		return (int_to_hex((uintptr_t) va_arg(args, void *), 1, 1));
	else if (*str == 'x')
		return (int_to_hex((uintptr_t) va_arg(args, unsigned int), 1, 0));
	else if (*str == 'X')
		return (int_to_hex((uintptr_t) va_arg(args, unsigned int), 0, 0));
	else if (*str == 'u')
		return (ft_unsigned_itoa(va_arg(args, unsigned int)));
	return (NULL);
}

static int	handle_conversion(int fd, const char *format, va_list args
	, t_print_state *p_state)
{
	char	*str;
	char	c[2];
	int		len_count;

	if (!format[p_state->i + 1])
		return (-1);
	str = handle_percent(&format[++(p_state->i)], args, c);
	if (!str)
		return (-1);
	len_count = ft_putstr(fd, str, format[p_state->i]);
	if (len_count == -1)
		return (-1);
	p_state->count += len_count;
	if (!(format[p_state->i] == 'c' || format[p_state->i] == 's'
			|| format[p_state->i] == '%'))
		free (str);
	return (0);
}

int	process_format(int fd, const char *format, va_list args)
{
	t_print_state	p_state;

	p_state = (t_print_state){0, 0};
	while (format[p_state.i])
	{
		if (format[p_state.i] == '%')
		{
			if (handle_conversion(fd, format, args, &p_state) == -1)
				return (-1);
		}
		else
		{
			if (write(fd, &format[p_state.i], 1) == -1)
				return (-1);
			p_state.count++;
		}
		p_state.i++;
	}
	return (p_state.count);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	args;

	if (!format)
		return (-1);
	va_start(args, format);
	count = process_format(1, format, args);
	va_end(args);
	return (count);
}
