#include "minirt.h"

static bool	is_visible(t_point *p, t_light *l, t_object **objects, size_t count)
{
	(void)p;
	(void)l;
	(void)objects;
	if (count % 2)
		return (true);
	else
		return (false);
}

int32_t	get_color(t_point *p, t_scene *s)
{
	int8_t	light_mask;
	double	ambient;
	double	light;

	ambient = s->ambient->intensity;
	light = get_light_intensity(p, s);
	light_mask = (ambient + light) * 255; //scale according to number of lights?
	return (p->object->color | light_mask);
}

double get_light_intensity(t_point *p, t_scene *s)
{
	double 	total;
	double	normal[3];
	double	light_to_point[3];

	total = 0;
	if(is_visible(p, s->light, s->objects, s->num_objects))
	{
			make_normal_vector(p);
			sub_vec(light_to_point, p->point, s->light->position);
			total += dot_prod(normal, light_to_point) / (abs_vec(light_to_point));
	}
	return (total);
}
