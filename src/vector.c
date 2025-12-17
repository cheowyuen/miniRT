/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:50:02 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/17 09:50:17 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//add together two vectors, store result in target. 
void	add_vec(double *target, double *a, double *b)
{
	target[0] = a[0] + b[0];
	target[1] = a[1] + b[1];
	target[2] = a[2] + b[2];
}

//subtract two vectors, store result in target. t = a - b
void	sub_vec(double *target, double *a, double *b)
{
	target[0] = a[0] - b[0];
	target[1] = a[1] - b[1];
	target[2] = a[2] - b[2];
}

//the dot product of two vectors
double	dot_prod(double *a, double *b)
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

//multiply vector with scalar
void	mult_vec(double *target, double *vec, double scale)
{
	target[0] = vec[0] * scale;
	target[1] = vec[1] * scale;
	target[2] = vec[2] * scale;
}

//returns the length of a vector
double	abs_vec(double *vec)
{
	return (sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
}
