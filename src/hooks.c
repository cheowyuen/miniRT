#include "minirt.h"

static void	update_image(t_scene *scene)
{
	(void)scene;
}

static void	move_camera(keys_t key, t_scene *scene)
{
	printf("The camera will move in direction %d\n", key);
	update_image(scene);
}

static void	pan_camera(keys_t key, t_scene *scene)
{
	printf("The camera will pan in direction %d\n", key);
	update_image(scene);
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
