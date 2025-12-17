/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:50:36 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/17 09:50:39 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	update_image(t_app *window)
{
	uint32_t	xy[2];
	int32_t		color;
	mlx_image_t	*new;

	ft_memset(xy, 0, sizeof(uint32_t) * 2);
	window->scene->camera->inverse_w = 1.0 / window->width;
	window->scene->camera->inverse_h = 1.0 / window->height;
	new = mlx_new_image(window->mlx, (uint32_t)window->width, \
(uint32_t)window->height);
	while (xy[0] < (uint32_t)window->width)
	{
		while (xy[1] < (uint32_t)window->height)
		{
			color = color_of_pixel(window->scene, xy[0], xy[1]);
			mlx_put_pixel(new, xy[0], xy[1], color);
			++xy[1];
		}
		xy[1] = 0;
		++xy[0];
	}
	mlx_delete_image(window->mlx, window->current);
	mlx_image_to_window(window->mlx, new, 0, 0);
	window->current = new;
	window->last_render_time = mlx_get_time();
}

static bool	is_pan_key(keys_t key)
{
	int				i;
	const keys_t	pans[6] = {MLX_KEY_I, MLX_KEY_J, MLX_KEY_K, MLX_KEY_L};

	i = -1;
	while (++i < 6)
	{
		if (key == pans[i])
			return (true);
	}
	return (false);
}

static bool	is_movement_key(keys_t key)
{
	int				i;
	const keys_t	movements[6] = {MLX_KEY_Q, MLX_KEY_W, MLX_KEY_E, \
MLX_KEY_A, MLX_KEY_S, MLX_KEY_D};

	i = -1;
	while (++i < 6)
	{
		if (key == movements[i])
			return (true);
	}
	return (false);
}

static void	cycle_selection(t_scene *s)
{
	if (s->selected_type == SELECT_OBJECT)
	{
		s->selected_index++;
		if (s->selected_index >= s->num_objects)
		{
			if (s->light)
				s->selected_type = SELECT_LIGHT;
			else
				s->selected_type = SELECT_CAMERA;
			s->selected_index = 0;
		}
	}
	else if (s->selected_type == SELECT_LIGHT)
		s->selected_type = SELECT_CAMERA;
	else if (s->selected_type == SELECT_CAMERA)
		s->selected_type = SELECT_OBJECT;
	print_selected_element(s);
}

void	handle_keys(mlx_key_data_t keydata, void *param)
{
	t_app	*window;

	window = (t_app *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		window->exit_code = 0;
		ft_exit(window);
	}
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
		cycle_selection(window->scene);
	if (is_movement_key(keydata.key) && keydata.action == MLX_PRESS)
		move_camera(keydata.key, window);
	if (is_pan_key(keydata.key) && keydata.action == MLX_PRESS)
		pan_camera(keydata.key, window);
	if ((keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN) && \
keydata.action == MLX_PRESS)
		resize_obj(keydata.key, window);
}
