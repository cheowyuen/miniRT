/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:55:43 by cwong             #+#    #+#             */
/*   Updated: 2025/11/13 09:26:00 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


//add safety checks: radius and height should remain > 0
//maybe change to percent change, so a multiplier instead of addition
void	resize_obj(keys_t key, t_app *window)
{
	t_scene		*s;
	t_object	*obj;
	double		step;

	s = window->scene;
	obj = s->objects[s->selected_index];
	step = 0.1;
	if (key == MLX_KEY_UP)
	{
		if (obj->type == SPHERE)
			obj->radius += step;
		else
			obj->height += step;
	}
	else if (key == MLX_KEY_DOWN)
	{
		if (obj->type == SPHERE && obj->radius > step)
			obj->radius -= step;
		else if (obj->type == CYLINDER && obj->height > step)
			obj->height -= step;
	}
	update_image(window);
}

void	handle_resize(int32_t width, int32_t height, void *param)
{
	t_app	*window;
	double	current_time;

	window = (t_app *)param;
	window->width = width;
	window->height = height;
	setup_geometry(window->scene->camera, false, width, height);
	window->resize_pending = true;
	current_time = mlx_get_time();
	if (current_time - window->last_render_time >= REDRAW_INTERVAL)
		update_image(window);
}

static void	cal_move(double *move, double *direction, double step)
{
	double	scaled[3];

	mult_vec(scaled, direction, step);
	add_vec(move, move, scaled);
}

static void	update_position(t_scene *s, double *move)
{
	if (s->selected_type == SELECT_OBJECT)
		add_vec(s->objects[s->selected_index]->position,
			s->objects[s->selected_index]->position, move);
	else if (s->selected_type == SELECT_LIGHT)
		add_vec(s->light->position, s->light->position, move);
	else if (s->selected_type == SELECT_CAMERA)
		add_vec(s->camera->position, s->camera->position, move);
}

void	move_camera(keys_t key, t_app *window)
{
	double		step;
	double		move[3];
	double		right[3];
	double		default_up[3];
	t_scene		*s;

	s = window->scene;
	step = 0.1;
	ft_bzero(move, sizeof(move));
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
