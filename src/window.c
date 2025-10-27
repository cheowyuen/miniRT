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

void	update_image(t_app *window)
{
	mlx_image_t	*new;
	uint32_t	xy[2];
	int32_t		color;
	
	new = mlx_new_image(window->mlx, HEIGHT, WIDTH);
	ft_memset(xy, 0, sizeof(uint32_t) * 2);
	while (xy[0] < HEIGHT)
	{
		while (xy[1] < WIDTH)
		{
			color = for_each_pixel(window->scene, xy[1], xy[0]);
			mlx_put_pixel(new, xy[1], xy[0], color);
			++xy[1];
		}
		++xy[0];
		xy[1] = 0;
	}
	mlx_delete_image(window->mlx, window->current);
	window->current = new;
	mlx_image_to_window(window->mlx, window->current, 0, 0);
}

void	make_window(t_scene *scene, t_app *window)
{
	window->scene = scene;
	window->mlx = mlx_init(HEIGHT, WIDTH, "miniRT", true);
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
	update_image(window);
}
