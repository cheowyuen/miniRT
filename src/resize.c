/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:33:25 by cwong             #+#    #+#             */
/*   Updated: 2025/11/20 12:01:03 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//add safety checks: radius and height should remain > 0
//maybe change to percent change, so a multiplier instead of addition
void	resize_obj(keys_t key, t_app *window)
{
	t_scene		*s;
	t_object	*obj;
	double		factor_up;
	double		factor_down;

	s = window->scene;
	obj = s->objects[s->selected_index];
	factor_up = 1.02;
	factor_down = 0.98;
	if (key == MLX_KEY_UP)
	{
		if (obj->type == SPHERE)
			obj->radius *= factor_up;
		else
			obj->height *= factor_up;
	}
	else if (key == MLX_KEY_DOWN)
	{
		if (obj->type == SPHERE && obj->radius * factor_down > 0.01)
			obj->radius *= factor_down;
		else if (obj->type == CYLINDER && obj->height * factor_down > 0.01)
			obj->height *= factor_down;
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
	setup_geometry(window->scene->camera, width, height);
	window->resize_pending = true;
	current_time = mlx_get_time();
	if (current_time - window->last_render_time >= REDRAW_INTERVAL)
		update_image(window);
}
