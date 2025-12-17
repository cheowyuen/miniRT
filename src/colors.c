/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:43:00 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/17 09:43:04 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//target must be an array of length 3
void	blend_colors(double *target, int32_t a, int32_t b)
{
	target[0] = (((a >> 24) & 0xFF) / 255.0f) * (((b >> 24) & 0xFF) / 255.0f);
	target[1] = (((a >> 16) & 0xFF) / 255.0f) * (((b >> 16) & 0xFF) / 255.0f);
	target[2] = (((a >> 8) & 0xFF) / 255.0f) * (((b >> 8) & 0xFF) / 255.0f);
}

int32_t	add_colors(double *color_a, double *color_b)
{
	uint8_t		channel[3];
	uint32_t	final;

	channel[0] = (uint8_t)(fmin((color_a[0] + color_b[0]), 1.0) * 255);
	channel[1] = (uint8_t)(fmin((color_a[1] + color_b[1]), 1.0) * 255);
	channel[2] = (uint8_t)(fmin((color_a[2] + color_b[2]), 1.0) * 255);
	final = (uint32_t)channel[0] << 24;
	final |= (uint32_t)channel[1] << 16;
	final |= (uint32_t)channel[2] << 8;
	final |= 0xFF;
	return ((int32_t)final);
}
