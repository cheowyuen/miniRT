/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:49:58 by cwong             #+#    #+#             */
/*   Updated: 2025/12/18 13:38:57 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	hit_points(double *hit, double *origin, double *direction, double t)
{
	mult_vec(hit, direction, t);
	add_vec(hit, hit, origin);
}

void	proj_perpendicular(double *vec, double *axis, double *result)
{
	double	dot;

	dot = dot_prod(vec, axis);
	mult_vec(result, axis, dot);
	sub_vec(result, vec, result);
}

void	make_point(t_point *p, double *position, t_object *object)
{
	int	i;

	p->object = object;
	i = -1;
	while (++i < 3)
		p->point[i] = position[i];
}

// only called by hit_cylinder, see vectors definition there
void	make_coeffs(double *coeffs, double vectors[4][3], double radius)
{
	coeffs[0] = dot_prod(vectors[1], vectors[1]);
	coeffs[1] = 2 * dot_prod(vectors[2], vectors[1]);
	coeffs[2] = dot_prod(vectors[2], vectors[2]) - (radius * radius);
	coeffs[3] = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
}
