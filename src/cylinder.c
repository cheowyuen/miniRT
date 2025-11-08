/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:51:46 by cwong             #+#    #+#             */
/*   Updated: 2025/11/08 18:05:34 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	cal_t(double a, double b, double discriminant, double *t)
{
	double	t1;
	double	t2;

	t1 = (-b + sqrt(discriminant)) / (2 * a);
	t2 = (-b - sqrt(discriminant)) / (2 * a);
	if (t1 > 0 && t2 > 0)
	{
		if (t1 < t2)
			*t = t1;
		else
			*t = t2;
	}
	else if (t1 > 0)
		*t = t1;
	else if (t2 > 0)
		*t = t2;
	else
		return (false);
	return (true);
}

static bool	hit_disk(double *center, t_ray ray, t_object *cylinder, double *t)
{
	t_vec	v;
	double	denom;
	double	dist2;

	sub_vec(v.oc, center, ray.origin);
	denom = dot_prod(ray.direction, cylinder->orientation);
	if (fabs(denom) > 1e-6)
	{
		*t = dot_prod(v.oc, cylinder->orientation) / denom;
		if (*t > 0)
		{
			hit_points(v.hit, ray.origin, ray.direction, *t);
			sub_vec(v.vec, v.hit, center);
			proj_perpendicular(v.vec, cylinder->orientation, v.perp);
			dist2 = dot_prod(v.perp, v.perp);
			if (dist2 <= cylinder->radius * cylinder->radius)
				return (true);
		}
	}
	return (false);
}

bool	hit_caps(t_ray ray, t_object *cylinder, double *t)
{
	double	cap_center[3];
	double	disk[3];

	mult_vec(cap_center, cylinder->orientation, cylinder->height / 2);
	sub_vec(disk, cylinder->position, cap_center);
	if (hit_disk(disk, ray, cylinder, t))
		return (true);
	add_vec(disk, cap_center, cylinder->position);
	if (hit_disk(disk, ray, cylinder, t))
		return (true);
	return (false);
}

static bool	hit_height(double t, t_ray ray, t_object *cylinder)
{
	double	ray_step[3];
	double	height;
	double	vec[3];

	mult_vec(ray_step, ray.direction, t);
	add_vec(vec, ray_step, ray.origin);
	sub_vec(vec, vec, cylinder->position);
	height = dot_prod(vec, cylinder->orientation);
	if (height < 0 || height > cylinder->height)
		return (false);
	return (true);
}

bool	hit_cylinder(t_ray ray, t_object *cylinder, double *t)
{
	double	oc[3];
	double	d_perp[3];
	double	oc_perp[3];
	double	d_perp2;
	double	temp_t;

	sub_vec(oc, ray.origin, cylinder->position);
	proj_perpendicular(ray.direction, cylinder->orientation, d_perp);
	proj_perpendicular(oc, cylinder->orientation, oc_perp);
	d_perp2 = dot_prod(d_perp, d_perp);
	if (d_perp2 < 1e-6)
		return (false);
	if (!hit_object(d_perp, oc_perp, cylinder->radius, &temp_t))
		return (false);
	if (!hit_height(temp_t, ray, cylinder))
		return (false);
	*t = temp_t;
	return (true);
}
