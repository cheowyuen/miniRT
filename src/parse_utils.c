#include "minirt.h"

static ssize_t	parse_whole_number(double *value, char *str, int *read, \
char delim)
{
	ssize_t	i;

	i = 0;
	while (str[i] && ft_isdigit((int)str[i]) && !isinf(*value))
	{
		*value = *value * 10 + str[i] - '0';
		++i;
	}
	if (i != 0 && (str[i] == '\0' || str[i] == delim) && !isinf(*value))
	{
		*read = *read + i;
		return (i);
	}
	if (str[i] != '.' || isinf(*value) || i == 0)
	{
		*read = -1;
		return (-1);
	}
	*read = *read + i + 1;
	return (i + 1);
}

static ssize_t	parse_fractions(double *value, char *str, int *read, char delim)
{
	double	divisor;
	ssize_t	i;

	divisor = 10.0 * copysign(1.0, *value);
	i = 0;
	while (str[i] && ft_isdigit((int)str[i]))
	{
		*value = *value + (str[i] - '0') / divisor;
		divisor *= 10;
		++i;
	}
	if (str[i] != delim && str[i] != '\0')
	{
		*read = -1;
		return (-1);
	}
	*read = *read + i + 1;
	return (i + 1);
}

double	ft_atod(char *line, int *read, char delimiter)
{
	double	value;
	int		sign;
	ssize_t	i;

	while (*line == ' ' && ++(*read))
		line++;
	value = 0.0;
	sign = 1;
	if (*(line++) == '-')
		sign = -1;
	else
		--line;
	*read += (sign == -1);
	i = parse_whole_number(&value, line, read, delimiter);
	if (i != -1)
	{
		line += i;
		if (parse_fractions(&value, line, read, delimiter) < 0)
			*read = -1;
	}
	return (value * sign);
}

ssize_t	parse_color(int32_t *color, char *str)
{
	int		c;
	int		val;
	ssize_t	i;

	while (*str == ' ')
		str++;
	c = -1;
	i = 0;
	while (++c < 3)
	{
		val = 0;
		if (!ft_isdigit(str[i]))
			return (-1);
		while (ft_isdigit(str[i]))
			val = val * 10 + (str[i++] - '0');
		if (val > 255 || (c < 2 && str[i++] != ',') || (c == 2 && str[i]))
			return (-1);
		*color = (*color << 8) | val;
	}
	*color = (*color << 8);
	return (i);
}

ssize_t	parse_triplet(double *target, char *str)
{
	const char	*delims = ",, ";
	double		temp;
	ssize_t		count;
	int			read;
	int			index;

	index = -1;
	count = 0;
	while (*str == ' ' && ++count)
		str++;
	while (++index < 3)
	{
		read = 0;
		target[index] = ft_atod(str, &read, delims[index]);
		if (read == -1 || (index < 2 && str[read] == '\0'))
			return (-1);
		str += read;
		count += read;
	}
	return (count);
}
