/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:55:43 by cwong             #+#    #+#             */
/*   Updated: 2025/12/18 12:19:24 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

	pct = 0.2;
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
		return (1.0);
	return (0.05);
}

static void	get_camera_move(keys_t key, t_scene *s, double step, double move[3])
{
	double	right[3];
	double	default_up[3];

	ft_bzero(move, sizeof(double) * 3);
	cal_right(right, default_up, s->camera->direction);
	if (key == MLX_KEY_Q)
		mult_vec(move, default_up, step);
	else if (key == MLX_KEY_E)
		mult_vec(move, default_up, -step);
	else if (key == MLX_KEY_A)
		mult_vec(move, right, -step);
	else if (key == MLX_KEY_D)
		mult_vec(move, right, step);
	else if (key == MLX_KEY_W)
		mult_vec(move, s->camera->direction, step);
	else if (key == MLX_KEY_S)
		mult_vec(move, s->camera->direction, -step);
}

void	move_camera(keys_t key, t_app *window)
{
	double		step;
	double		move[3];
	t_scene		*s;
	double		revert[3];

	s = window->scene;
	step = get_step(s);
	get_camera_move(key, s, step, move);
	update_position(s, move);
	if (!check_in_objects(s, false))
	{
		mult_vec(revert, move, -1);
		update_position(s, revert);
		return ;
	}
	update_image(window);
}
