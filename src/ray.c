#include "minirt.h"

void	hit_points(double *hit, double *origin, double *direction, double t)
{
	hit[0] = origin[0] + t * direction[0];
	hit[1] = origin[1] + t * direction[1];
	hit[2] = origin[2] + t * direction[2];
}

bool	hit_plane(t_ray ray, t_object *plane, double *t)
{
	double	po[3];
	double	a;
	double	b;
	
	sub_vec(po, plane->position, ray.origin);
	a = dot_prod(po, plane->orientation);
	b= dot_prod(ray.direction, plane->orientation);
	if (fabs(b) < 1e-9) // plane is parallel to ray
		return (false);
	*t = a / b;
	// if ray is too close to origin
	// if (*t < 1e-6)
	// 	return (false);
	// return (true);
	return (*t >= 0);
}

bool	hit_sphere(t_ray ray, t_object *sphere, double *t)
{
	double	oc[3];
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;

	sub_vec(oc, ray.origin, sphere->position);
	a = dot_prod(ray.direction, ray.direction);
	b = 2 * dot_prod(oc, ray.direction);
	c = dot_prod(oc, oc) - (sphere->radius * sphere->radius);
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

void normalize(double *d)
{
	double	length;

	length = abs_vec(d);
	d[0] /= length;
	d[1] /= length;
	d[2] /= length;
}

t_ray	make_ray(double *origin, int cx, int cy)
{
	t_ray	ray;
	double	vw;
	double	vh;

	ray.origin[0] = origin[0];
	ray.origin[1] = origin[1];
	ray.origin[2] = origin[2];
	// assuming 1x1 viewport, can upgrade to fov based viewport
	// fov_x = fov * M_PI / 180.0 
	// vw = 2 * tan(fov_x / 2)
	// vh = vw * double(HEIGHT) / WIDTH
	vw = 1;
	vh = 1;
	ray.direction[0] = (cx + 0.5) * vw / (double)WIDTH - vw / 2;
	ray.direction[1] = vh / 2 - (cy + 0.5) * vh / (double)HEIGHT;
	ray.direction[2] = 1;
	normalize(ray.direction);
	return (ray);
}

void	for_each_pixel(t_scene *scene)
{
	int			cx;
	int			cy;
	t_ray		ray;
	double		t;
	double		closest_t;
	t_object	*closest_obj;
	double		hit[3];
	int			i;
	bool		is_hit;

	// cx = 0;
	// while (cx < WIDTH)
	// {
	// 	cy = 0;
	// 	while (cy < HEIGHT)
	// 	{
			cx = 800;
			cy = 500;
			ray = make_ray(scene->camera->position, cx, cy);
			closest_t = INFINITY;
			closest_obj = NULL;
			i = 0;
			// while (i < scene.num_objects)
			// {
				is_hit = false;
				if (scene->objects[i]->type == SPHERE)
					is_hit = hit_sphere(ray, scene->objects[i], &t);
				else if (scene->objects[i]->type == PLANE)
					is_hit = hit_plane(ray, scene->objects[i], &t);
				if (is_hit && t < closest_t)
				{
					closest_t = t;
					closest_obj = scene->objects[i];
				}
			// }
			if (closest_obj)
			{
				hit_points(hit, ray.origin, ray.direction, closest_t);
				printf("Hit point: (%f, %f, %f)\n", hit[0], hit[1], hit[2]);
				// apply lighting
			}
			else
			{ 
				printf("No hit\n");
				// apply background colour
			}
			// cy++;
	// 	}
	// 	cx++;
	// }
}
