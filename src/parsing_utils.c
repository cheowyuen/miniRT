/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:25:08 by cwong             #+#    #+#             */
/*   Updated: 2025/11/08 11:47:57 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

void	cal_vec(double *direction, double *up, double *right, bool is_init)
{
	double		default_up[3];

	normalize(direction);
	if (is_init)
		cal_right(right, default_up, direction);
	else
	{
		cross_vec(right, direction, up);
		normalize(right);
	}
	cross_vec(up, right, direction);
	normalize(up);
}

void	setup_geometry(t_camera *c, bool is_init, int32_t width, int32_t height)
{
	double		right[3];
	double		vw;
	double		vh;
	double		fov_rad;
	int			i;

	cal_vec(c->direction, c->up, right, is_init);
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
