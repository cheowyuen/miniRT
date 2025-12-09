/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:25:08 by cwong             #+#    #+#             */
/*   Updated: 2025/12/09 16:55:27 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_camera(t_camera *c, int32_t width, int32_t height)
{
	double		default_up[3];

	default_up[0] = 0.0;
	default_up[1] = 1.0;
	default_up[2] = 0.0;
	if (fabs(dot_prod(c->direction, default_up)) > 0.999)
	{
		default_up[0] = 1.0;
		default_up[1] = 0.0;
		default_up[2] = 0.0;
	}
	cross_vec(c->right, c->direction, default_up);
	normalize(c->right);
	cross_vec(c->up, c->right, c->direction);
	normalize(c->up);
	c->w = (double)width;
	c->h = (double)height;
	c->inverse_w = 1.0 / (double)width;
	c->inverse_h = 1.0 / (double)height;
}

//In perspective camera, c->zero holds the direction c-to-pixel-(0,0)
static void	setup_perspective(t_camera *c, int32_t width, int32_t height)
{
	double		vw;
	double		vh;
	double		fov_rad;
	int			i;

	fov_rad = c->fov * M_PI / 180.0;
	vw = 2.0 * tan(fov_rad / 2);
	vh = vw / ((double)width / height);
	i = -1;
	while (++i < 3)
	{
		c->delta_x[i] = c->right[i] * vw / (width);
		c->delta_y[i] = -c->up[i] * vh / (height);
		c->zero[i] = c->direction[i] - \
c->delta_x[i] * ((width) / 2.0) - c->delta_y[i] * ((height) / 2.0);
	}
}

//In orthographic camera, c->zero holds the location of pixel (0,0)
static void	setup_orthographic(t_camera *c, int32_t width, int32_t height)
{
	const double	vw = 20.0;
	double			vh;
	int				i;

	vh = vw / ((double)width / height);
	i = -1;
	while (++i < 3)
	{
		c->delta_x[i] = c->right[i] * vw / width;
		c->delta_y[i] = -c->up[i] * vh / height;
		c->zero[i] = c->position[i] - (c->right[i] * vw / 2.0) + \
(c->up[i] * vh / 2.0);
	}
}

void	setup_geometry(t_camera *c, int32_t width, int32_t height)
{
	init_camera(c, width, height);
	if (c->fov < 1.0)
		setup_orthographic(c, width, height);
	else if (c->fov <= 179.0)
		setup_perspective(c, width, height);
}
