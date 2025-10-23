#include "minirt.h"

void	hit_points(double *hit, double *origin, double *direction, double t)
{
	hit[0] = origin[0] + t * direction[0];
	hit[1] = origin[1] + t * direction[1];
	hit[2] = origin[2] + t * direction[2];
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
		if (t2 < t1)
			*t = t2;
		else
			*t = t1;
	}
	else if (t1 > 0)
		*t = t1;
	else if (t2 > 0)
		*t = t2;
	else
		return (false);
	return (true);
}

t_ray	make_ray(double *origin, int cx, int cy)
{
	t_ray	ray;

	ray.origin[0] = origin[0];
	ray.origin[1] = origin[1];
	ray.origin[2] = origin[2];
	// assuming 1x1 viewport
	// center the viewport at (0,0)
	// vx=(cx+0.5).(vw/cw)-vw/2
	// vy=vh/2-(ch+0.5).(vh/ch)
	ray.direction[0] = (double)cx / WIDTH;
	ray.direction[1] = (double)cy / HEIGHT;
	ray.direction[2] = 1;
	// normalize(ray.direction);
	return (ray);
}

void	for_each_pixel(t_scene scene)
{
	int			cx;
	int			cy;
	t_ray		ray;
	double		t;
	double		closest_t;
	t_object	*closest_sphere;
	double		hit[3];
	int			i;

	// cx = 0;
	// while (cx < WIDTH)
	// {
	// 	cy = 0;
	// 	while (cy < HEIGHT)
	// 	{
			cx = 800;
			cy = 500;
			ray = make_ray(scene.camera->position, cx, cy);
			closest_t = INFINITY;
			closest_sphere = NULL;
			i = 0;
			// while (i < scene.num_objects)
			// {
				if (scene.objects[i]->type == SPHERE)
				{
					if (hit_sphere(ray, scene.objects[i], &t))
					{
						if (t < closest_t)
						{
							closest_t = t;
							closest_sphere = scene.objects[i];
						}
					}
				}
			// }
			if (closest_sphere)
			{
				hit_points(hit, ray.origin, ray.direction, closest_t);
				// apply lighting
			}
			else
			{
				// apply background colour
			}
			// cy++;
	// 	}
	// 	cx++;
	// }
}
