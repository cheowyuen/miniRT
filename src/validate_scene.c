#include "minirt.h"

static bool	intersect_cylinder(t_object *obj, double *pos)
{
	double		d;
	double		axis[3];
	double		mult[3];
	double		vec[3];
	
	sub_vec(vec, pos, obj->position);
	d = dot_prod(vec, obj->orientation);
	if (d < -obj->height / 2 || d > obj->height / 2)
		return (false);
	mult_vec(mult, obj->orientation, d);
	add_vec(axis, obj->position, mult);
	sub_vec(vec, pos, axis);
	if (abs_vec(vec) <= obj->radius + 1e-2)
		return (true);
	return (false);
}

static void	validate_position(t_scene *s, double *pos, char *err_msg)
{
	size_t		i;
	t_object	*obj;
	double		vec[3];

	i = 0;
	while (i < s->num_objects)
	{
		obj = s->objects[i];
		if (obj->type == SPHERE)
		{
			sub_vec(vec, pos, obj->position);
			if (abs_vec(vec) <= obj->radius + 1e-2)
				parse_error(err_msg, NULL, s, NULL);
		}
		else if (obj->type == CYLINDER)
		{
			if (intersect_cylinder(obj, pos))
				parse_error(err_msg, NULL, s, NULL);
		}
		i++;
	}
}

void	check_in_objects(t_scene *s)
{
	t_camera	*c;
	int			i;
	int			y;
	int			x;
	double		pos[3];

	c = s->camera;
	validate_position(s, c->position, "Invalid camera position");
	if (c->fov == 0)
	{
		y = 0;
		while (y < HEIGHT)
		{
			x = 0;
			while (x < WIDTH)
			{
				i = 0;
				while (i < 3)
				{
					pos[i] = c->zero[i] + c->delta_x[i] * x + c->delta_y[i] * y;
					i++;
				}
				validate_position(s, pos, "Invalid viewport position");
				x++;
			}
			y++;
		}
	}
}

static bool	is_normal(double *vec)
{
	double length;

	length = abs_vec(vec);
	if (length > 1.0)
	{
		if (vec[0] >= -1.0 && vec[0] <= 1.0 && vec[1] >= -1.0 && vec[1] <= 1.0\
 && vec[2] >= -1.0 && vec[2] <= 1.0 && length > 0.0001)
		{
			printf("Warning: components of vector in range [-1,1] but vector \
is not normalized, normalizing\n");
			normalize(vec);
		}
		else 
			return (false);
	}
	if (length < 0.0001)
		return (false);
	return (true);
}

void	check_requirements(t_scene *scene)
{
	int i;

	if (!scene->camera)
		parse_error("No camera defined", NULL, scene, NULL);
	if (!scene->ambient)
		parse_error("No ambient light defined", NULL, scene, NULL);
	if (scene->num_objects == 0)
		parse_error("No objects defined", NULL, scene, NULL);
	if (!is_normal(scene->camera->direction))
		parse_error("Camera direction not normalized", NULL, scene, NULL);
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		if ((scene->objects[i]->type == PLANE || scene->objects[i]->type == \
CYLINDER) && !is_normal(scene->objects[i]->orientation))
			parse_error("Object orientation not normalized", NULL, scene, NULL);
	}
}
