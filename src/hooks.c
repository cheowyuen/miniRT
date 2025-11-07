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

static void resize_obj(keys_t key, t_app *window)
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

static void	cal_move(double *move, double *direction, double step)
{
	double	scaled[3];

	mult_vec(scaled, direction, step);
	add_vec(move, move, scaled);
}

static void	move_camera(keys_t key, t_app *window)
{
	double 		step;
	double 		move[3];
	double 		right[3];
	double 		default_up[3];
	t_scene		*s;
	
	s = window->scene;
	step = 0.1;
	ft_bzero(move, sizeof(move));
	cal_right(right, default_up, s->camera->direction);
	if (key == MLX_KEY_Q)
		cal_move(move,  default_up, step);
	else if (key == MLX_KEY_E)
		cal_move(move,  default_up, -step);
	else if (key == MLX_KEY_A)
		cal_move(move, right, -step);
	else if (key == MLX_KEY_D)
		cal_move(move, right, step);
	else if (key == MLX_KEY_W)
		cal_move(move,  s->camera->direction, step); 
	else if (key == MLX_KEY_S)
		cal_move(move,  s->camera->direction, -step);
	if (s->selected_type == SELECT_OBJECT)
		add_vec(s->objects[s->selected_index]->position, s->objects[s->selected_index]->position, move);
	else if (s->selected_type == SELECT_LIGHT)
		add_vec(s->lights[s->selected_index]->position, s->lights[s->selected_index]->position, move);
	else if (s->selected_type == SELECT_CAMERA)
		add_vec(s->camera->position, s->camera->position, move);
	update_image(window);
}

static void	rotate_vec(double *direction, double *axis, double angle, double *result)
{
	double	a[3];
	double	b[3];
	double	c[3];
	double	d[3];
	double	e[3];
	double	f[3];

	mult_vec(a, direction, cos(angle));
	cross_vec(b, axis, direction);
	mult_vec(c, b, sin(angle));
	add_vec(d, a, c);
	mult_vec(e, axis, dot_prod(axis, direction));
	mult_vec(f, e, 1 - cos(angle));
	add_vec(result, d, f);
}

static void	pan_camera(keys_t key, t_app *window)
{
	double 		angle;
	double 		vec[3];
	double 		right[3];
	t_scene		*s;
	double		direction[3];
	double		up[3];
	t_object	*obj;

	s = window->scene;
	obj = s->objects[s->selected_index];
	if (s->selected_type == SELECT_OBJECT &&  obj->type != SPHERE)
	{
		copy_vec(direction, obj->orientation);
		copy_vec(up, obj->up);
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		copy_vec(direction, s->camera->direction);
		copy_vec(up, s->camera->up);
	}
	else
		return ;
	angle = 0.05;
	cross_vec(right, direction, up);
	normalize(right);
	if (key == MLX_KEY_I)
		rotate_vec(direction, up, -angle, vec);
	else if (key == MLX_KEY_K)
		rotate_vec(direction, up, angle, vec);
	else if (key == MLX_KEY_J)
		rotate_vec(direction, right, -angle, vec);
	else if (key == MLX_KEY_L)
		rotate_vec(direction, right, angle, vec);
	else if (key == MLX_KEY_U)
		rotate_vec(up, direction, angle, vec);
	else if (key == MLX_KEY_O)
		rotate_vec(up, direction, -angle, vec);
	if (s->selected_type == SELECT_OBJECT && obj->type != SPHERE)
	{
		if (key == MLX_KEY_U || key == MLX_KEY_O)
			copy_vec(obj->up, vec);
		else
			copy_vec(obj->orientation, vec);
		setup_geometry_obj(obj, false);
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		if (key == MLX_KEY_U || key == MLX_KEY_O)
			copy_vec(s->camera->up, vec);
		else
			copy_vec(s->camera->direction, vec);
		setup_geometry(s->camera, false);
	}
	update_image(window);
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

void	cycle_selection(t_scene *s)
{
	if (s->selected_type == SELECT_OBJECT)
	{
		s->selected_index++;
		if (s->selected_index >= s->num_objects)
		{
			s->selected_type = SELECT_LIGHT;
			s->selected_index = 0;
		}
	}
	else if (s->selected_type == SELECT_LIGHT)
	{
		s->selected_index++;
		if (s->selected_index >= s->num_lights)
		{
			s->selected_type = SELECT_CAMERA;
			s->selected_index = 0;
		}
	}
	else if (s->selected_type == SELECT_CAMERA)
	{
		s->selected_index = 0;
		s->selected_type = SELECT_OBJECT;
	}
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
	if ((keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN) && keydata.action == MLX_PRESS)
		resize_obj(keydata.key, window);
}
