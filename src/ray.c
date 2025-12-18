/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:50:17 by cwong             #+#    #+#             */
/*   Updated: 2025/12/18 16:56:22 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	hit_object(double *dir, double *oc, double r, double *t)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;

	a = dot_prod(dir, dir);
	b = 2 * dot_prod(oc, dir);
	c = dot_prod(oc, oc) - (r * r);
	discriminant = b * b - 4 * a * c;
	if (discriminant < -EPSILON)
		return (false);
	return (cal_t(a, b, discriminant, t));
}

bool	hit_plane(t_ray ray, t_object *plane, double *t)
{
	double	oc[3];
	double	a;
	double	b;

	sub_vec(oc, plane->position, ray.origin);
	a = dot_prod(oc, plane->orientation);
	b = dot_prod(ray.direction, plane->orientation);
	if (fabs(b) < EPSILON)
		return (false);
	*t = a / b;
	if (*t < EPSILON)
		return (false);
	return (*t >= 0);
}

static bool	object_intersect(t_scene *scene, int i, t_ray ray, double *t)
{
	double	oc[3];
	double	temp[2];
	bool	hit;

	temp[0] = INFINITY;
	temp[1] = INFINITY;
	if (scene->objects[i]->type == SPHERE)
	{
		sub_vec(oc, ray.origin, scene->objects[i]->position);
		return (hit_object(ray.direction, oc, scene->objects[i]->radius, t));
	}
	else if (scene->objects[i]->type == PLANE)
		return (hit_plane(ray, scene->objects[i], t));
	else if (scene->objects[i]->type == CYLINDER)
	{
		hit = hit_cylinder(ray, scene->objects[i], &temp[0]);
		if (hit_caps(ray, scene->objects[i], &temp[1]) || hit)
		{
			*t = fmin(temp[0], temp[1]);
			return (true);
		}
	}
	return (false);
}

static void	loop_objects(t_scene *scene, t_ray ray, t_hit *hit)
{
	double		t;
	int			i;

	i = 0;
	while (i < (int)scene->num_objects)
	{
		if (object_intersect(scene, i, ray, &t) && t < hit->t)
		{
			hit->t = t;
			hit->obj = scene->objects[i];
		}
		i++;
	}
}

int32_t	color_of_pixel(t_scene *scene, uint32_t cx, uint32_t cy)
{
	t_ray		ray;
	t_hit		hit_info;
	double		hit[3];
	t_point		p;

	ray = make_ray(scene->camera, cx, cy);
	hit_info.t = INFINITY;
	hit_info.obj = NULL;
	loop_objects(scene, ray, &hit_info);
	if (hit_info.obj)
	{
		hit_points(hit, ray.origin, ray.direction, hit_info.t);
		make_point(&p, hit, hit_info.obj);
		return (get_color(&p, scene));
	}
	else
		return (255);
}
