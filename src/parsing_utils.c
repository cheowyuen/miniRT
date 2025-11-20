/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:25:08 by cwong             #+#    #+#             */
/*   Updated: 2025/11/20 12:16:34 by nraatika         ###   ########.fr       */
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
