#include "minirt.h"

void	update_image(t_app *window)
{
	uint32_t	xy[2];
	int32_t		color;
	mlx_image_t	*new;

	ft_memset(xy, 0, sizeof(uint32_t) * 2);
	new = mlx_new_image(window->mlx, WIDTH, HEIGHT);
	while (xy[0] < WIDTH)
	{
		while (xy[1] < HEIGHT)
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
}

static void	move_camera(keys_t key, t_scene *scene)
{
	printf("The camera will move in direction %d\n", key);
	(void)scene;
//	update_image(scene);
}

static void	pan_camera(keys_t key, t_scene *scene)
{
	printf("The camera will pan in direction %d\n", key);
	(void)scene;
//	update_image(scene);
}

static bool	is_pan_key(keys_t key)
{
	int				i;
	const keys_t	pans[6] = {MLX_KEY_U, MLX_KEY_I, MLX_KEY_O, MLX_KEY_J,\
 MLX_KEY_K, MLX_KEY_L};

	i = -1;
	while (++i < 6)
	{
		if (key == pans[i])
			return true;
	}
	return false;
}

static bool	is_movement_key(keys_t key)
{
	int				i;
	const keys_t	movements[6] = {MLX_KEY_Q, MLX_KEY_W, MLX_KEY_E, MLX_KEY_A,\
 MLX_KEY_S, MLX_KEY_D};

	i = -1;
	while (++i < 6)
	{
		if (key == movements[i])
			return true;
	}
	return false;
}

void	handle_resize(int32_t width, int32_t height, void* param)
{
	t_app	*window;

//	recalculate viewport to match new size of screen, keeping top left pixel constant?
//	or keep rendering everything, but only showing the new selection if smaller?
	window = (t_app *)param;
	mlx_set_window_size(window->mlx, width, height);
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
	if (is_movement_key(keydata.key) && keydata.action == MLX_PRESS)
		move_camera(keydata.key, window->scene);
	if (is_pan_key(keydata.key) && keydata.action == MLX_PRESS)
		pan_camera(keydata.key, window->scene);
}
