#include "minirt.h"

void	copy_vec(double *target, double *vec)
{
	target[0] = vec[0];
	target[1] = vec[1];
	target[2] = vec[2];
}

void	cross_vec(double *target, double *a, double *b)
{
	target[0] = a[1] * b[2] - a[2] * b[1];
	target[1] = a[2] * b[0] - a[0] * b[2];
	target[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double *d)
{
	double	length;

	length = abs_vec(d);
	d[0] /= length;
	d[1] /= length;
	d[2] /= length;
}

void	cal_right(double *right, double *default_up, double *direction)
{
	default_up[0] = 0.0;
	default_up[1] = 1.0;
	default_up[2] = 0.0;
	if (fabs(dot_prod(direction, default_up)) > 0.999)
	{
		default_up[0] = 1.0;
		default_up[1] = 0.0;
		default_up[2] = 0.0;
	}
	cross_vec(right, direction, default_up);
	normalize(right);
}

void	setup_geometry(t_camera *c, bool is_init, int32_t width, int32_t height)
{
	double		default_up[3];
	double		right[3];
	double		vw;
	double		vh;
	double		fov_rad;
	int			i;

	normalize(c->direction);
	if (is_init)
		cal_right(right, default_up, c->direction);
	else
	{
		cross_vec(right, c->direction, c->up);
		normalize(right);
	}
	cross_vec(c->up, right, c->direction);
	normalize(c->up);
	fov_rad = c->fov * M_PI / 180.0;
	vw = 2.0 * tan(fov_rad / 2);
	vh = vw / ((double)width / height);
	i = 0;
	while (i < 3)
	{
		c->delta_x[i] = right[i] * vw / width;
		c->delta_y[i] = -c->up[i] * vh / height;
		c->zero_coords[i] = c->position[i] + c->direction[i]
			- c->delta_x[i] * (width / 2.0)
			- c->delta_y[i] * (height / 2.0);
		i++;
	}
}

void	setup_geometry_obj(t_object *obj, bool is_init)
{
	double		default_up[3];
	double		right[3];

	normalize(obj->orientation);
	if (is_init)
		cal_right(right, default_up, obj->orientation);
	else
	{
		cross_vec(right, obj->orientation, obj->up);
		normalize(right);
	}
	cross_vec(obj->up, right,  obj->orientation);
	normalize(obj->up);
}
