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
}
