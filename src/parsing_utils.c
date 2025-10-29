#include "minirt.h"

void	cross_vec(double *target, double *a, double *b)
{
	target[0] = a[1] * b[2] - a[2] * b[1];
	target[1] = a[2] * b[0] - a[0] * b[2];
	target[2] = a[0] * b[1] - a[1] * b[0];
}

void	setup_geometry(t_camera *c)
{
	double		default_up[3];
	double		right[3];
	double		up[3];
	double		vw;
	double		vh;
	double		fov_rad;
	int			i;

	normalize(c->direction);
	default_up[0] = 0.0;
	default_up[1] = 1.0;
	default_up[2] = 0.0;
	if (fabs(dot_prod(c->direction, default_up)) > 0.999)
	{
		default_up[0] = 1.0;
		default_up[1] = 0.0;
		default_up[2] = 0.0;
	}
	cross_vec(right, c->direction, default_up);
	normalize(right);
	cross_vec(up, right, c->direction);
	normalize(up);
	fov_rad = c->fov * M_PI / 180.0;
	vw = 2.0 * tan(fov_rad / 2);
	vh = vw / ((double)WIDTH / HEIGHT);
	i = 0;
	while (i < 3)
	{
		c->delta_x[i] = right[i] * vw / WIDTH;
		c->delta_y[i] = -up[i] * vh / HEIGHT;
		c->zero_coords[i] = c->position[i] + c->direction[i]
			- c->delta_x[i] * (WIDTH / 2.0)
			- c->delta_y[i] * (HEIGHT / 2.0);
		i++;
	}
}