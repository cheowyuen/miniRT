/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:51:46 by cwong             #+#    #+#             */
/*   Updated: 2025/11/10 16:32:51 by nraatika         ###   ########.fr       */
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
		if (*t > 1e-6)
		{
			hit_points(v.hit, ray.origin, ray.direction, *t);
			sub_vec(v.vec, v.hit, center);
			dist2 = dot_prod(v.vec, v.vec);
			if (dist2 <= cylinder->radius * cylinder->radius)
				return (true);
		}
	}
	*t = INFINITY;
	return (false);
}

bool	hit_caps(t_ray ray, t_object *cylinder, double *t)
{
	double	cap_center[3];
	double	disk[3];
	double	temp[2];
	bool	hit;

	hit = false;
	temp[0] = INFINITY;
	temp[1] = INFINITY;
	mult_vec(cap_center, cylinder->orientation, cylinder->height / 2);
	sub_vec(disk, cylinder->position, cap_center);
	if (hit_disk(disk, ray, cylinder, &temp[0]))
		hit = true;
	add_vec(disk, cylinder->position, cap_center);
	if (hit_disk(disk, ray, cylinder, &temp[1]))
		hit = true;
	if (hit)
		*t = fmin(temp[0], temp[1]);
	return (hit);
}

static bool	hit_height(double t, t_ray ray, t_object *cylinder)
{
	double	ray_step[3];
	double	height;
	double	vec[3];
	double	half_height;

	mult_vec(ray_step, ray.direction, t);
	add_vec(vec, ray_step, ray.origin);
	sub_vec(vec, vec, cylinder->position);
	height = dot_prod(vec, cylinder->orientation);
	half_height = cylinder->height / 2.0;
	if (height < -half_height || height > half_height)
		return (false);
	return (true);
}

bool	hit_cylinder(t_ray ray, t_object *cylinder, double *t)
{
	double	v[4][3];
	double	coeffs[4];
	
	v[3][0] = INFINITY;
	sub_vec(v[0], ray.origin, cylinder->position);
	proj_perpendicular(ray.direction, cylinder->orientation, v[1]);
	proj_perpendicular(v[0], cylinder->orientation, v[2]);
	coeffs[0] = dot_prod(v[1], v[1]);
	coeffs[1] = 2 * dot_prod(v[2], v[1]);
	coeffs[2] = dot_prod(v[2], v[2]) - (cylinder->radius * cylinder->radius);
	coeffs[3] =  coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	if (fabs(coeffs[0]) < 1e-6 || coeffs[3] < 0)
		return (false);
	coeffs[3] = sqrt(coeffs[3]);
	v[3][1] = (-coeffs[1] + coeffs[3]) / (2 * coeffs[0]);
	v[3][2] = (-coeffs[1] - coeffs[3]) / (2 * coeffs[0]);
	if (v[3][2] > 1e-6 && hit_height(v[3][2], ray, cylinder))
		v[3][0] = v[3][2];
	if (v[3][1] > 1e-6 && hit_height(v[3][1], ray, cylinder) && v[3][1] < v[3][2])
		v[3][0] = v[3][1];
	if (v[3][0] < INFINITY)
	{
		*t = v[3][0];
		return (true);
	}
	return (false);
}
