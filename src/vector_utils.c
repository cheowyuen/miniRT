/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:23:20 by cwong             #+#    #+#             */
/*   Updated: 2025/12/09 18:01:00 by cwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	copy_vec(double *target, double *vec)
{
	target[0] = vec[0];
	target[1] = vec[1];
	target[2] = vec[2];
}

void	cross_vec(double *target, double *a, double *b)
{
	target[0] = b[1] * a[2] - b[2] * a[1];
	target[1] = b[2] * a[0] - b[0] * a[2];
	target[2] = b[0] * a[1] - b[1] * a[0];
}

void	normalize(double *d)
{
	double	length;

	length = abs_vec(d);
	d[0] /= length;
	d[1] /= length;
	d[2] /= length;
}
