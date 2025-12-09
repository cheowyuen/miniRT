/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:56:49 by cwong             #+#    #+#             */
/*   Updated: 2025/12/09 12:07:22 by nraatika         ###   ########.fr       */
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


static void	reset_rotation(t_app *window, double direction[3])
{
	t_scene	*s;

	s = window->scene;
	if (s->selected_type == SELECT_OBJECT &&\
 s->objects[s->selected_index]->type != SPHERE)
		copy_vec(s->objects[s->selected_index]->orientation, direction);
	else if (s->selected_type == SELECT_CAMERA)
		copy_vec(s->camera->direction, direction);
}

/*
	vectors array holds:
	0: final vector passed as new camera direction
	1: right vector used during calculations
	2: direction of rotation
	3: up vector used during calculations
	4: old direction to be returned if something went wrong
*/
void	pan_camera(keys_t key, t_app *window)
{
	double		vectors[5][3];
	t_scene		*s;
	t_object	*obj;

	s = window->scene;
	obj = s->objects[s->selected_index];
	if (!(is_rotatable(s, vectors[2], vectors[3], vectors[1])))
		return ;
	copy_vec(vectors[4], vectors[2]);
	if (key == MLX_KEY_I)
		rotate_vec(vectors[2], vectors[1], ROTATION_ANGLE, vectors[0]);
	else if (key == MLX_KEY_K)
		rotate_vec(vectors[2], vectors[1], -ROTATION_ANGLE, vectors[0]);
	else if (key == MLX_KEY_J)
		rotate_vec(vectors[2], vectors[3], -ROTATION_ANGLE, vectors[0]);
	else if (key == MLX_KEY_L)
		rotate_vec(vectors[2], vectors[3], ROTATION_ANGLE, vectors[0]);
	update_vec(vectors[0], window);
	if (!check_in_objects(s, false))
	{
		reset_rotation(window, vectors[4]);
		return ;
	}
	update_image(window);
}
