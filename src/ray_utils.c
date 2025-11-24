/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:49:58 by cwong             #+#    #+#             */
/*   Updated: 2025/11/24 16:18:52 by nraatika         ###   ########.fr       */
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

void	add_scaled_vecs(double pos[3], t_camera *camera, int cx, int cy)
{
	pos[0] = camera->zero[0] + camera->delta_x[0] * cx
		+ camera->delta_y[0] * cy;
	pos[1] = camera->zero[1] + camera->delta_x[1] * cx
		+ camera->delta_y[1] * cy;
	pos[2] = camera->zero[2] + camera->delta_x[2] * cx
		+ camera->delta_y[2] * cy;
}

void	make_point(t_point *p, double *position, t_object *object)
{
	int	i;

	p->object = object;
	i = -1;
	while (++i < 3)
		p->point[i] = position[i];
}

// used by hit_cylinder, see vectors definition there
void	make_coeffs(double *coeffs, double **vectors, double radius)
{
	coeffs[0] = dot_prod(vectors[1], vectors[1]);
	coeffs[1] = 2 * dot_prod(vectors[2], vectors[1]);
	coeffs[2] = dot_prod(vectors[2], vectors[2]) - (radius * radius);
	coeffs[3] = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
}
