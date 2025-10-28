#include "minirt.h"

static ssize_t	parse_whole_number(double *value, char *str, int *read)
{
	ssize_t	i;

	i = 0;
	while (str[i] && ft_isdigit((int)str[i]) && !isinf(*value))
	{
		*value =  *value * 10 + str[i] - '0';
		++i;
	}
	if (str[i] != '.' || isinf(*value))
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

	divisor = 10.0 * copysign(1.0, *value); //if value was negative, fractions need to be negative as well
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

double ft_atod(char *line, int *read, char delimiter)
{
	double	value;
	int		sign;
	ssize_t	i;

	value = 0.0;
	sign = 1;
	if (*(line++) == '-')
		sign = -1;
	else
		--line;
	*read += (sign == -1);
	i = parse_whole_number(&value, line, read);
	if (i == -1)
		return (value);
	line = line + i;
	i = parse_fractions(&value, line, read, delimiter);
	if (i == -1)
		return (value);
	return (value * sign);
}

ssize_t	parse_triplet(double *target, char *str)
{
	double	temp;
	ssize_t	count;
	int		read;
	int		index;

	index = 0;
	count = 0;
	while (index < 2)
	{
		read = 0;
		temp = ft_atod(str, &read, ',');
		if (read == -1 || str[read] == '\0')
			return (-1);
 		str += read;
		count += read;
		target[index++] = temp;
	}
	temp = ft_atod(str, &read, ' ');
	if (read == -1)
		return (-1);
	count += read;
	target[index] = temp;
	return (count);
}
