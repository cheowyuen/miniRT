#include "minirt.h"

void	hit_points(double *hit, double *origin, double *direction, double t)
{
	hit[0] = origin[0] + t * direction[0];
	hit[1] = origin[1] + t * direction[1];
	hit[2] = origin[2] + t * direction[2];
}

void	cal_perpendicular(double *x, double *y, double *perp)
{
	double	z;

	z = dot_prod(x, y);
	perp[0] = x[0] - z * y[0];
	perp[1] = x[1] - z * y[1];
	perp[2] = x[2] - z * y[2];
}

bool	hit_cap(double *pos, t_ray ray, t_object *cylinder, double *t)
{
	double	co[3];
	double	denom;
	double	hit[3];
	double	vec[3];
	double	perp[3];
	double	dist;

	sub_vec(co, pos, ray.origin);
	denom =  dot_prod(ray.direction, cylinder->orientation);
	if (fabs(denom) > 1e-6)
	{
		*t = dot_prod(co, cylinder->orientation) / denom;
		if (*t > 0)
		{
			hit_points(hit, ray.origin, ray.direction, *t);
			sub_vec(vec, hit, pos);
			cal_perpendicular(vec, cylinder->orientation, perp);
			// proj = dot_prod(vec, cylinder->orientation);
			// perp[0] = vec[0] - proj * cylinder->orientation[0];
			// perp[1] = vec[1] - proj * cylinder->orientation[1];
			// perp[2] = vec[2] - proj * cylinder->orientation[2];
			dist = dot_prod(perp, perp);
			if (dist <= cylinder->radius * cylinder->radius)
				return (true);
		}
	}
	return (false);
}

bool	hit_caps(t_ray ray, t_object *cylinder, double *t)
{
	double	cap_center[3];
	
	// sub_vec(co, cylinder->position, ray.origin);
	// denom =  dot_prod(ray.direction, cylinder->orientation);
	// if (fabs(denom) > 1e-6)
	// {
	// 	*t = dot_prod(co, cylinder->orientation) / denom;
	// 	if (*t > 0)
	// 	{
	// 		hit_points(hit, ray.origin, ray.direction, *t);
	// 		sub_vec(vec, hit, cylinder->position);
	// 		cal_perpendicular(vec, cylinder->orientation, &perp);
	// 		// proj = dot_prod(vec, cylinder->orientation);
	// 		// perp[0] = vec[0] - proj * cylinder->orientation[0];
	// 		// perp[1] = vec[1] - proj * cylinder->orientation[1];
	// 		// perp[2] = vec[2] - proj * cylinder->orientation[2];
	// 		dist = dot_prod(perp, perp);
	// 		if (dist <= cylinder->radius * cylinder->radius)
	// 			return (true);
	// 	}
	// }
	if (hit_cap(cylinder->position, ray, cylinder, t))
		return (true);
	cap_center[0] = cylinder->position[0] + cylinder->orientation[0] * cylinder->height;
	cap_center[1] = cylinder->position[1] + cylinder->orientation[1] * cylinder->height;
	cap_center[2] = cylinder->position[2] + cylinder->orientation[2] * cylinder->height;
	if (hit_cap(cap_center, ray, cylinder, t))
		return (true);
	return (false);
}

bool	hit_height(double t, t_ray ray, t_object *cylinder)
{
	double x[3];
	double y;
	double vec[3];

	x[0] = t * ray.direction[0];
	x[1] = t * ray.direction[1];
	x[2] = t * ray.direction[2];
	vec[0] = ray.origin[0] + x[0] - cylinder->position[0];
	vec[1] = ray.origin[1] + x[1] - cylinder->position[1];
	vec[2] = ray.origin[2] + x[2] - cylinder->position[2];
	y = dot_prod(vec, cylinder->orientation);
	if (y < 0 || y > cylinder->height)
		return (false);
	return (true);
}

bool	hit_object(double *x, double *y, double z, double *t)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;

	a = dot_prod(x, x);
	b = 2 * dot_prod(y, x);
	c = dot_prod(y, y) - (z * z);
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

	sub_vec(oc, ray.origin, cylinder->position);
	// a = dot_prod(ray.direction, cylinder->orientation);
	// d_perp[0] = ray.direction[0] - a * cylinder->orientation[0];
	// d_perp[1] = ray.direction[1] - a * cylinder->orientation[1];
	// d_perp[2] = ray.direction[2] - a * cylinder->orientation[2];
	cal_perpendicular(ray.direction, cylinder->orientation, d_perp);
	// a = dot_prod(oc, cylinder->orientation);
	// oc_perp[0] = oc[0] - a * cylinder->orientation[0];
	// oc_perp[1] = oc[1] - a * cylinder->orientation[1];
	// oc_perp[2] = oc[2] - a * cylinder->orientation[2];
	cal_perpendicular(oc, cylinder->orientation, oc_perp);
	return (hit_object(d_perp, oc_perp, cylinder->radius, t));
	// a = dot_prod(d_perp, d_perp);
	// b = 2 * dot_prod(d_perp, oc_perp);
	// c = dot_prod(oc_perp, oc_perp) - (cylinder->radius * cylinder->radius);
	// discriminant = b * b - 4 * a * c;
	// if (discriminant < 0)
	// 	return (false);
	// t1 = (-b + sqrt(discriminant)) / (2 * a);
	// t2 = (-b - sqrt(discriminant)) / (2 * a);
	// if (!hit_height(t1, ray, cylinder) && !hit_height(t2, ray, cylinder))
	// 	return (false);
	// if (t1 > 0 && t2 > 0)
	// {
	// 	if (t1 < t2)
	// 		*t = t1;
	// 	else
	// 		*t = t2;
	// }
	// else if (t1 > 0)
	// 	*t = t1;
	// else if (t2 > 0)
	// 	*t = t2;
	// else 
	// 	return (false);
	// return (true);
}

bool	hit_plane(t_ray ray, t_object *plane, double *t)
{
	double	po[3];
	double	a;
	double	b;
	
	sub_vec(po, plane->position, ray.origin);
	a = dot_prod(po, plane->orientation);
	b= dot_prod(ray.direction, plane->orientation);
	if (fabs(b) < 1e-6) // plane is parallel to ray
		return (false);
	*t = a / b;
	// if ray is too close to origin
	// if (*t < 1e-6)
	// 	return (false);
	// return (true);
	return (*t >= 0);
}

// bool	hit_sphere(t_ray ray, t_object *sphere, double *t)
// {
// 	double	oc[3];
// 	double	a;
// 	double	b;
// 	double	c;
// 	double	discriminant;
// 	double	t1;
// 	double	t2;

// 	sub_vec(oc, ray.origin, sphere->position);
// 	a = dot_prod(ray.direction, ray.direction);
// 	b = 2 * dot_prod(oc, ray.direction);
// 	c = dot_prod(oc, oc) - (sphere->radius * sphere->radius);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return (false);
// 	t1 = (-b + sqrt(discriminant)) / (2 * a);
// 	t2 = (-b - sqrt(discriminant)) / (2 * a);
// 	if (t1 > 0 && t2 > 0)
// 	{
// 		if (t1 < t2)
// 			*t = t1;
// 		else
// 			*t = t2;
// 	}
// 	else if (t1 > 0)
// 		*t = t1;
// 	else if (t2 > 0)
// 		*t = t2;
// 	else
// 		return (false);
// 	return (true);
// }

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
	double		oc[3];

	// cx = 0;
	// while (cx < WIDTH)
	// {
	// 	cy = 0;
	// 	while (cy < HEIGHT)
	// 	{
			cx = 800;
			cy = 500;
			ray = make_ray(scene->camera->position, cx, cy);
			ray.origin[0] = -50.0;  // directly above cylinder
			ray.origin[1] = 10.0;   // above top by 4 units
			ray.origin[2] = 50.0;   // same Z as cylinder
			ray.direction[0] = 0.0;
			ray.direction[1] = -1.0; // pointing downward
			ray.direction[2] = 0.0;
			closest_t = INFINITY;
			closest_obj = NULL;
			i = 2;
			// while (i < scene.num_objects)
			// {
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
					t_object *cyl = scene->objects[i];
					printf("Ray origin: %.2f %.2f %.2f\n", ray.origin[0], ray.origin[1], ray.origin[2]);
					printf("Ray dir: %.4f %.4f %.4f\n", ray.direction[0], ray.direction[1], ray.direction[2]);
					printf("Cylinder base: %.2f %.2f %.2f\n", cyl->position[0], cyl->position[1], cyl->position[2]);
					printf("Cylinder axis: %.4f %.4f %.4f\n", cyl->orientation[0], cyl->orientation[1], cyl->orientation[2]);
					printf("Radius=%.2f Height=%.2f\n", cyl->radius, cyl->height);
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
