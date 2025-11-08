/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:49:58 by cwong             #+#    #+#             */
/*   Updated: 2025/11/08 17:57:49 by cwong            ###   ########.fr       */
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

t_ray	make_ray(double *origin, double pixel_pos[3])
{
	t_ray	ray;

	copy_vec(ray.origin, origin);
	sub_vec(ray.direction, pixel_pos, origin);
	normalize(ray.direction);
	return (ray);
}

void	add_scaled_vecs(double pos[3], t_camera *camera, int cx, int cy)
{
	pos[0] = camera->zero_coords[0] + camera->delta_x[0] * cx
		+ camera->delta_y[0] * cy;
	pos[1] = camera->zero_coords[1] + camera->delta_x[1] * cx
		+ camera->delta_y[1] * cy;
	pos[2] = camera->zero_coords[2] + camera->delta_x[2] * cx
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
