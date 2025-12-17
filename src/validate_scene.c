/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:00:05 by cwong             #+#    #+#             */
/*   Updated: 2025/12/16 16:42:44 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static bool	validate_viewport(t_scene *s, t_camera *c, int y, bool close)
{
	int		x;
	int		i;
	double	pos[3];

	x = 0;
	while (x < WIDTH)
	{
		i = 0;
		while (i < 3)
		{
			pos[i] = c->zero[i] + c->delta_x[i] * x + c->delta_y[i] * y;
			i++;
		}
		if (!validate_position(s, pos, "Invalid viewport position", close))
			return (false);
		x++;
	}
	return (true);
}

bool	check_in_objects(t_scene *s, bool close)
{
	t_camera	*c;
	int			y;

	c = s->camera;
	if (!validate_position(s, c->position, "Invalid camera position", close))
		return (false);
	if (c->fov == 0)
	{
		y = 0;
		while (y < HEIGHT)
		{
			if (!validate_viewport(s, c, y, close))
				return (false);
			y++;
		}
	}
	return (true);
}

static bool	is_normal(double *vec)
{
	double	length;

	length = abs_vec(vec);
	if (length > 1.0001 || length < 0.9999)
	{
		if (vec[0] >= -1.0 && vec[0] <= 1.0 && vec[1] >= -1.0 && vec[1] <= 1.0 \
&& vec[2] >= -1.0 && vec[2] <= 1.0)
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
	int	i;

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
