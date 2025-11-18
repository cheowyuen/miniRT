#include "minirt.h"

static bool	check_side_of_plane(t_point *p, t_light *l, t_camera *cam)
{
	double	point_to_cam[3];
	double	point_to_light[3];
	double	dot_cam;
	double	dot_light;

	sub_vec(point_to_cam, cam->position, p->point);
	sub_vec(point_to_light, l->position, p->point);
	dot_cam = dot_prod(p->normal, point_to_cam);
	dot_light = dot_prod(p->normal, point_to_light);
	if (dot_cam < 0.0f && dot_light < 0.0f)
	{//if both dot products are both negative, both light and camera are on
	//"wrong side of plane"
		mult_vec(p->normal, p->normal, -1.0f);
		return (true);
	}
	else if (dot_cam > 0.0f && dot_light > 0.0f)
		return (true);
	return (false);
}

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
//to offset the point so it doesn't self-shadow, move point a tiny bit in the 
//direction of the normal
static bool	is_visible(t_point *p, t_light *l, t_scene *s)
{
	double	distance;
	double	temp[3];
	double	offset_point[3];
	size_t	i;
	t_ray	ray;

	if (p->object->type == PLANE && !check_side_of_plane(p, l, s->camera))
		return (false);
	mult_vec(temp, p->normal, 0.01f); //move the offsetting to the point calculation for efficency?
	add_vec(offset_point, p->point, temp);
	sub_vec(temp, l->position, offset_point);
	distance = abs_vec(temp);
	ray = make_ray(offset_point, l->position);
	i = 0;
	while (i < s->num_objects)
	{
		if (min_distance(&ray, s->objects[i]) < distance)
			return (false);
		++i;
	}
	return (true);
}

int32_t	get_color(t_point *p, t_scene *s)
{
	double	ambient[3];
	double	light[3];

	blend_colors(ambient, p->object->color, s->ambient->color);
	mult_vec(ambient, ambient, s->ambient->intensity);
	if (s->light)
	{
		blend_colors(light, p->object->color, s->light->color);
		mult_vec(light, light, get_light_intensity(p, s));
		return (add_colors(ambient, light));
	}
	ft_bzero(light, 3 * sizeof(double));
	return (add_colors(ambient, light));
}

double get_light_intensity(t_point *p, t_scene *s)
{
	double	dist;
	double	attenuation;
	double 	total;
	double	normal[3];
	double	light_to_point[3];

	total = 0;
	make_normal_vector(p);
	if(is_visible(p, s->light, s))
	{
		mult_vec(normal, p->normal, 1.0f / abs_vec(p->normal));
		sub_vec(light_to_point, s->light->position, p->point);
		dist = abs_vec(light_to_point);
		normalize(light_to_point);
		attenuation = 1.0 / (1.0 + LINEAR * dist + QUAD * dist * dist);
		total = dot_prod(normal, light_to_point) * attenuation;
		total *= s->light->intensity;
		if (total < 0)
            total = 0;
	}
	return (total);
}
