#include "minirt.h"

int32_t	get_color(t_point *p, t_scene *s){
	int32_t base;
	double	ambient;
	double	light;

	base = p->object->color;
	ambient = s->ambient->intensity;
	light = get_light_intensity(p, s);
}

double get_light_intensity(t_point *p, t_scene *s)
{
	double 	total;
	double	normal[3];
	double	light_to_point[3];
	int		i;

	total = 0;
	i = -1;
	while (++i < s->num_lights)
	{
		if(is_visible(p, s->lights[i], s->objects))
		{
			make_normal_vector(p, normal);
			sub_vec(light_to_point, p->point, s->lights[i]->position);
			total += dot_prod(normal, light_to_point) / (abs_vec(light_to_point));
		}
	}
}
