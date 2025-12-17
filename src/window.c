/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:42:10 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/17 09:42:17 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"	

void	free_window(t_app *window)
{
	if (window && window->mlx)
	{
		if (window->current)
			mlx_delete_image(window->mlx, window->current);
		if (window->mlx->window)
		{
			mlx_close_window(window->mlx);
		}
		mlx_terminate(window->mlx);
	}
}

void	check_pending_resize(void *param)
{
	t_app	*window;

	window = (t_app *)param;
	if (!window->resize_pending)
		return ;
	else if (!mlx_is_mouse_down(window->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		window->resize_pending = false;
		update_image(window);
	}
}

void	print_selected_element(t_scene *s)
{
	const char	*o_strs[3] = {"sphere", "plane", "cylinder"};
	const char	*format = "Selected %s at %f,%f,%f\n";
	double		*temp;

	if (s->selected_type == SELECT_OBJECT)
	{
		temp = s->objects[s->selected_index]->position;
		printf(format, o_strs[s->objects[s->selected_index]->type], temp[0], \
temp[1], temp[2]);
	}
	else if (s->selected_type == SELECT_LIGHT)
	{
		temp = s->light->position;
		printf(format, "light", temp[0], temp[1], temp[2]);
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		temp = s->camera->position;
		printf(format, "camera", temp[0], temp[1], temp[2]);
	}
}

void	make_window(t_scene *scene, t_app *window)
{
	window->scene = scene;
	window->width = WIDTH;
	window->height = HEIGHT;
	window->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (window->mlx)
		window->current = mlx_new_image(window->mlx, HEIGHT, WIDTH);
	else
	{
		window->exit_code = 1;
		ft_exit(window);
	}
	if (!window->current)
	{
		window->exit_code = 2;
		ft_exit(window);
	}
	mlx_set_window_limit(window->mlx, 100, 100, -1, -1);
	mlx_key_hook(window->mlx, handle_keys, window);
	mlx_close_hook(window->mlx, ft_exit, window);
	mlx_resize_hook(window->mlx, handle_resize, window);
	if (! mlx_loop_hook(window->mlx, check_pending_resize, window))
	{
		window->exit_code = 1;
		ft_exit(window);
	}
}
