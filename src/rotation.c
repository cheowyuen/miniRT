/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:56:49 by cwong             #+#    #+#             */
/*   Updated: 2025/11/24 12:36:00 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	rotate_vec(double *direction, double *axis, double angle,
	double *result)
{
	double	a[3];
	double	b[3];
	double	c[3];
	double	d[3];
	double	e[3];

	mult_vec(a, direction, cos(angle));
	cross_vec(b, axis, direction);
	mult_vec(c, b, sin(angle));
	add_vec(d, a, c);
	mult_vec(e, axis, dot_prod(axis, direction) * (1 - cos(angle)));
	add_vec(result, d, e);
}

static bool	is_rotatable(t_scene *s, double *direction, double *up,
	double *right)
{
	t_object	*obj;

	obj = s->objects[s->selected_index];
	if (s->selected_type == SELECT_OBJECT && obj->type != SPHERE)
	{
		copy_vec(direction, obj->orientation);
		copy_vec(up, obj->up);
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		copy_vec(direction, s->camera->direction);
		copy_vec(up, s->camera->up);
	}
	else
		return (false);
	cross_vec(right, direction, up);
	normalize(right);
	return (true);
}

static void	update_vec(double *vec, t_app *window)
{
	t_scene		*s;
	t_object	*obj;
	double		right[3];

	s = window->scene;
	obj = s->objects[s->selected_index];
	if (s->selected_type == SELECT_OBJECT && obj->type != SPHERE)
	{
		copy_vec(obj->orientation, vec);
		cal_vec(obj->orientation, obj->up, right, false);
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		copy_vec(s->camera->direction, vec);
		setup_geometry(s->camera, window->width, window->height);
	}
}

void	pan_camera(keys_t key, t_app *window)
{
	double		vec[3];
	double		right[3];
	t_scene		*s;
	double		direction[3];
	double		up[3];
	bool		valid;
	double		old_dir[3];
	t_object	*obj;

	s = window->scene;
	obj = s->objects[s->selected_index];
	if (!(is_rotatable(s, direction, up, right)))
		return ;
	copy_vec(old_dir, direction);
	if (key == MLX_KEY_I)
		rotate_vec(direction, right, ROTATION_ANGLE, vec);
	else if (key == MLX_KEY_K)
		rotate_vec(direction, right, -ROTATION_ANGLE, vec);
	else if (key == MLX_KEY_J)
		rotate_vec(direction, up, -ROTATION_ANGLE, vec);
	else if (key == MLX_KEY_L)
		rotate_vec(direction, up, ROTATION_ANGLE, vec);
	update_vec(vec, window);
	valid = check_in_objects(s, false);
	if (!valid)
	{
		if (s->selected_type == SELECT_OBJECT && obj->type != SPHERE)
			copy_vec(obj->orientation, old_dir);
		else if (s->selected_type == SELECT_CAMERA)
			copy_vec(s->camera->direction, old_dir);
		return ;
	}
	update_image(window);
}
