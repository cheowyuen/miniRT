/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_vectors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:45:21 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/18 16:56:22 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	check which surface of the cylinder point is on, and write the corresponding
	normal vector to result
*/
static void	cylinder_normal(t_point *p)
{
	double	center_to_point[3];
	double	temp[3];
	double	length;

	sub_vec(center_to_point, p->point, p->object->position);
	length = dot_prod(center_to_point, p->object->orientation);
	if (fabs(fabs(length) - (p->object->height / 2)) < EPSILON)
		mult_vec(p->normal, p->object->orientation, copysign(1.0, length));
	else
	{
		mult_vec(temp, p->object->orientation, length);
		sub_vec(p->normal, center_to_point, temp);
		normalize(p->normal);
	}
}

//store the normal vector at point p into p->normal
void	make_normal_vector(t_point *p)
{
	if (p->object->type == PLANE)
		mult_vec(p->normal, p->object->orientation, 1);
	if (p->object->type == SPHERE)
	{
		sub_vec(p->normal, p->point, p->object->position);
		mult_vec(p->normal, p->normal, 1.0 / abs_vec(p->normal));
	}
	if (p->object->type == CYLINDER)
		cylinder_normal(p);
}
