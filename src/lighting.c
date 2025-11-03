#include "minirt.h"

static double	min_distance(t_ray *ray, t_object *object)
{
	double	temp[3];
	double	t;
	bool	is_hit;

	t = INFINITY;
	is_hit = false;
	if (object->type == SPHERE)
	{
		sub_vec(temp, ray->origin, object->position);
		is_hit = hit_object(ray->direction, temp, object->radius, &t);
	}
	else if (object->type == PLANE)
		is_hit = hit_plane(*ray, object, &t);
	else if (object->type == CYLINDER)
	{
		if (hit_cylinder(*ray, object, &t))
			is_hit = true;
		else if (hit_caps(*ray, object, &t))
			is_hit = true;
	}
	if (is_hit)
		return (t);
	return (INFINITY);
}

//goes through the list of objects: if the one with the smallest distance is 
//less than the distance to the light, return false, else return true
static bool	is_visible(t_point *p, t_light *l, t_object **objects, size_t count)
{
	double	distance;
	double	temp[3];
	size_t	i;
	t_ray	ray;

	i = 0;
	sub_vec(temp, l->position, p->point);
	distance = abs_vec(temp);
	ray = make_ray(p->point, l->position);
	while (i < count)
	{
		if (min_distance(&ray, objects[i]) < distance)
			return (false);
		++i;
	}
	return (true);
}

int32_t	get_color(t_point *p, t_scene *s)
{
	int		light_mask;
	double	ambient;
	double	light;

	//return (p->object->color | 255);
	ambient = s->ambient->intensity;
	light = get_light_intensity(p, s);
	light_mask = (ambient + light) * 255; //scale according to number of lights?
	if (light_mask < 0)
		light_mask = 0;
	if (light_mask > 255)
		light_mask = 255;
	return (p->object->color | light_mask);
}

double get_light_intensity(t_point *p, t_scene *s)
{
	double 	total;
	double	normal[3];
	double	light_to_point[3];
	int		i;

	total = 0;
	i = -1;
	while (++i < (int)(s->num_lights))
	{
		if(is_visible(p, s->lights[i], s->objects, s->num_objects))
		{
			make_normal_vector(p);
			mult_vec(normal, p->normal, 1.0/abs_vec(p->normal));
			sub_vec(light_to_point, p->point, s->lights[i]->position);
			normalize(light_to_point);
			total += dot_prod(normal, light_to_point) / \
(abs_vec(light_to_point) * abs_vec(light_to_point));
		}
	}
	return (total);
}
