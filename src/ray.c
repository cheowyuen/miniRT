#include "minirt.h"

void	hit_points(double *hit, double *origin, double *direction, double t)
{
	hit[0] = origin[0] + t * direction[0];
	hit[1] = origin[1] + t * direction[1];
	hit[2] = origin[2] + t * direction[2];
}

void	proj_perpendicular(double *vec, double *axis, double *result)
{
	double	dot;

	dot = dot_prod(vec, axis);
	result[0] = vec[0] - dot * axis[0];
	result[1] = vec[1] - dot * axis[1];
	result[2] = vec[2] - dot * axis[2];
}

bool	hit_disk(double *center, t_ray ray, t_object *cylinder, double *t)
{
	double	oc[3];
	double	denom;
	double	hit[3];
	double	vec[3];
	double	perp[3];
	double	dist2;

	sub_vec(oc, center, ray.origin);
	denom =  dot_prod(ray.direction, cylinder->orientation);
	if (fabs(denom) > 1e-6)
	{
		*t = dot_prod(oc, cylinder->orientation) / denom;
		if (*t > 0)
		{
			hit_points(hit, ray.origin, ray.direction, *t);
			sub_vec(vec, hit, center);
			proj_perpendicular(vec, cylinder->orientation, perp);
			dist2 = dot_prod(perp, perp);
			if (dist2 <= cylinder->radius * cylinder->radius)
				return (true);
		}
	}
	return (false);
}

bool	hit_caps(t_ray ray, t_object *cylinder, double *t)
{
	double	cap_center[3];
	
	if (hit_disk(cylinder->position, ray, cylinder, t))
		return (true);
	cap_center[0] = cylinder->position[0] + cylinder->orientation[0] * cylinder->height;
	cap_center[1] = cylinder->position[1] + cylinder->orientation[1] * cylinder->height;
	cap_center[2] = cylinder->position[2] + cylinder->orientation[2] * cylinder->height;
	if (hit_disk(cap_center, ray, cylinder, t))
		return (true);
	return (false);
}

bool	hit_height(double t, t_ray ray, t_object *cylinder)
{
	double ray_step[3];
	double height;
	double vec[3];

	ray_step[0] = t * ray.direction[0];
	ray_step[1] = t * ray.direction[1];
	ray_step[2] = t * ray.direction[2];
	vec[0] = ray.origin[0] + ray_step[0] - cylinder->position[0];
	vec[1] = ray.origin[1] + ray_step[1] - cylinder->position[1];
	vec[2] = ray.origin[2] + ray_step[2] - cylinder->position[2];
	height = dot_prod(vec, cylinder->orientation);
	if (height < 0 || height > cylinder->height)
		return (false);
	return (true);
}

bool	hit_object(double *dir, double *oc, double r, double *t)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;

	a = dot_prod(dir, dir);
	b = 2 * dot_prod(oc, dir);
	c = dot_prod(oc, oc) - (r * r);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (false);
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

bool	hit_plane(t_ray ray, t_object *plane, double *t)
{
	double	oc[3];
	double	a;
	double	b;
	
	sub_vec(oc, plane->position, ray.origin);
	a = dot_prod(oc, plane->orientation);
	b = dot_prod(ray.direction, plane->orientation);
	if (fabs(b) < 1e-6) // plane is parallel to ray
		return (false);
	*t = a / b;
	// if ray is too close to origin
	// if (*t < 1e-6)
	// 	return (false);
	// return (true);
	return (*t >= 0);
}

t_ray	make_ray(double *origin, double pixel_pos[3])
{
	t_ray	ray;

	ray.origin[0] = origin[0];
	ray.origin[1] = origin[1];
	ray.origin[2] = origin[2];
	ray.direction[0] = pixel_pos[0] - origin[0];
	ray.direction[1] = pixel_pos[1] - origin[1];
	ray.direction[2] = pixel_pos[2] - origin[2];
	normalize(ray.direction);
	return (ray);
}

void add_scaled_vecs(double pos[3], t_camera *camera, int cx, int cy)
{
    pos[0] = camera->zero_coords[0] + camera->delta_x[0] * cx + camera->delta_y[0] * cy;
    pos[1] = camera->zero_coords[1] + camera->delta_x[1] * cx + camera->delta_y[1] * cy;
    pos[2] = camera->zero_coords[2] + camera->delta_x[2] * cx + camera->delta_y[2] * cy;
}

static void	make_point(t_point *p, double *position, t_object *object)
{
	int	i;

	p->object = object;
	i = -1;
	while (++i < 3)
		p->point[i] = position[i];
}

int32_t	color_of_pixel(t_scene *scene, uint32_t cx, uint32_t cy)
{
	t_ray		ray;
	double		t;
	double		closest_t;
	t_object	*closest_obj;
	double		hit[3];
	int			i;
	bool		is_hit;
	double		oc[3];
	double		pixel_pos[3];
	t_point		p;

	add_scaled_vecs(pixel_pos, scene->camera, cx, cy);
	ray = make_ray(scene->camera->position, pixel_pos);
	closest_t = INFINITY;
	closest_obj = NULL;
	i = 0;
	while (i < (int)scene->num_objects)
	{
		is_hit = false;
		if (scene->objects[i]->type == SPHERE)
		{
			sub_vec(oc, ray.origin, scene->objects[i]->position);
			is_hit = hit_object(ray.direction, oc, scene->objects[i]->radius, &t);
		}
		else if (scene->objects[i]->type == PLANE)
			is_hit = hit_plane(ray, scene->objects[i], &t);
		else if (scene->objects[i]->type == CYLINDER)
		{
			if (hit_cylinder(ray, scene->objects[i], &t))
				is_hit = true;
			else if (hit_caps(ray, scene->objects[i], &t))
				is_hit = true;
		}
		if (is_hit && t < closest_t)
		{
			closest_t = t;
			closest_obj = scene->objects[i];
		}
		++i;
	}
	if (closest_obj)
	{
		hit_points(hit, ray.origin, ray.direction, closest_t);
//		printf("Hit point: (%f, %f, %f)\n", hit[0], hit[1], hit[2]);
		make_point(&p, hit, closest_obj);
		return (get_color(&p, scene));
	}
	else
	{ 
//		printf("No hit\n");
		// apply background colour
		return (255);
	}
}
