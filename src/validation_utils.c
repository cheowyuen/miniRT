/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:50:44 by cwong             #+#    #+#             */
/*   Updated: 2025/12/10 01:09:07 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static bool	handle_error(char *str, t_scene *s, bool close)
{
	if (close)
		parse_error(str, NULL, s, NULL);
	else
	{
		printf("Error\n%s\n", str);
		return (false);
	}
	return (false);
}

static bool	intersect_cylinder(t_object *obj, double *pos)
{
	double		d;
	double		axis[3];
	double		mult[3];
	double		vec[3];

	sub_vec(vec, pos, obj->position);
	d = dot_prod(vec, obj->orientation);
	if (d < -obj->height / 2 || d > obj->height / 2)
		return (false);
	mult_vec(mult, obj->orientation, d);
	add_vec(axis, obj->position, mult);
	sub_vec(vec, pos, axis);
	if (abs_vec(vec) < obj->radius - 1e-6)
		return (true);
	return (false);
}

bool	validate_position(t_scene *s, double *pos, char *str, bool close)
{
	size_t		i;
	t_object	*obj;
	double		vec[3];

	i = 0;
	while (i < s->num_objects)
	{
		obj = s->objects[i];
		if (obj->type == SPHERE)
		{
			sub_vec(vec, pos, obj->position);
			if (abs_vec(vec) < obj->radius - 1e-6)
				return (handle_error(str, s, close));
		}
		else if (obj->type == CYLINDER)
		{
			if (intersect_cylinder(obj, pos))
				return (handle_error(str, s, close));
		}
		i++;
	}
	return (true);
}
