/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:55:43 by cwong             #+#    #+#             */
/*   Updated: 2025/11/17 11:22:10 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	cal_move(double *move, double *direction, double step)
{
	double	scaled[3];

	mult_vec(scaled, direction, step);
	add_vec(move, move, scaled);
}

static void	update_position(t_scene *s, double *move)
{
	t_object	*obj;

	if (s->selected_type == SELECT_OBJECT)
	{
		obj = s->objects[s->selected_index];
		add_vec(obj->position, obj->position, move);
	}
	else if (s->selected_type == SELECT_LIGHT)
		add_vec(s->light->position, s->light->position, move);
	else if (s->selected_type == SELECT_CAMERA)
		add_vec(s->camera->position, s->camera->position, move);
}

static double	get_step(t_scene *s)
{
	double		pct;
	t_object	*obj;

	pct = 0.1;
	if (s->selected_type == SELECT_OBJECT)
	{
		obj = s->objects[s->selected_index];
		if (obj->type == SPHERE)
			return (obj->radius * pct);
		if (obj->type == PLANE)
			return (1.0);
		if (obj->type == CYLINDER)
			return (obj->height * pct);
	}
	else if (s->selected_type == SELECT_LIGHT)
		return (1.0);
	else if (s->selected_type == SELECT_CAMERA)
		return (0.05);
	return (0.05);
}

void	move_camera(keys_t key, t_app *window)
{
	double		step;
	double		move[3];
	double		right[3];
	double		default_up[3];
	t_scene		*s;

	s = window->scene;
	ft_bzero(move, sizeof(move));
	step = get_step(s);
	cal_right(right, default_up, s->camera->direction);
	if (key == MLX_KEY_Q)
		cal_move(move, default_up, step);
	else if (key == MLX_KEY_E)
		cal_move(move, default_up, -step);
	else if (key == MLX_KEY_A)
		cal_move(move, right, -step);
	else if (key == MLX_KEY_D)
		cal_move(move, right, step);
	else if (key == MLX_KEY_W)
		cal_move(move, s->camera->direction, step);
	else if (key == MLX_KEY_S)
		cal_move(move, s->camera->direction, -step);
	update_position(s, move);
	update_image(window);
}
