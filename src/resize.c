/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:33:25 by cwong             #+#    #+#             */
/*   Updated: 2025/12/18 12:23:52 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	apply_resize(t_object *obj, keys_t key, double factor_up
	, double factor_down)
{
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
}

void	resize_obj(keys_t key, t_app *window)
{
	t_scene		*s;
	t_object	*obj;
	double		old_radius;
	double		old_height;

	s = window->scene;
	if (s->selected_type != SELECT_OBJECT)
		return ;
	obj = s->objects[s->selected_index];
	old_radius = obj->radius;
	old_height = obj->height;
	apply_resize(obj, key, 1.05, 0.95);
	if (!check_in_objects(s, false))
	{
		obj->radius = old_radius;
		obj->height = old_height;
		return ;
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
