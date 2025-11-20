#include "minirt.h"

t_ray	make_ray_perspective(t_camera *cam, uint32_t cx, uint32_t cy)
{
	t_ray	ray;
	int		i;
	double	pixel_pos[3];

	add_scaled_vecs(pixel_pos, cam, cx, cy);
	copy_vec(ray.origin, cam->position);
	i = -1;
	while (++i < 3)
	{
		ray.direction[i] = cam->zero[i] 
			+ (cam->delta_x[i] * cx) 
			+ (cam->delta_y[i] * cy);
	}
	normalize(ray.direction);
	return (ray);
}

t_ray	make_ray_orthographic(t_camera *cam, uint32_t cx, uint32_t cy)
{
	t_ray	ray;
	int		i;

	copy_vec(ray.direction, cam->direction); 
	i = -1;
	while (++i < 3)
	{
			ray.origin[i] = cam->zero[i] + (cam->delta_x[i] * cx) \
+ (cam->delta_y[i] * cy);
	}
	return (ray);
}

t_ray	make_ray_spherical(t_camera *cam, uint32_t cx, uint32_t cy)
{
	t_ray	ray;
	double	angles[2]; // theta, phi
	double	xy[2];
	double	local_dir[3];

	copy_vec(ray.origin, cam->position);
	xy[0] = (double)cx * cam->inverse_w - 0.5;
	xy[1] = (double)cy * cam->inverse_h - 0.5;
	angles[0] = xy[0] * M_PI; 
	angles[1] = xy[1] * M_PI * (cam->h * cam->inverse_w);
	local_dir[0] = sin(angles[0]) * cos(angles[1]);
	local_dir[1] = -sin(angles[1]);
	local_dir[2] = cos(angles[0]) * cos(angles[1]);
	int i = -1;
	while (++i < 3)
	{
		ray.direction[i] = (local_dir[0] * cam->right[i]) + \
		                   (local_dir[1] * cam->up[i]) + \
		                   (local_dir[2] * cam->direction[i]);
	}
	normalize(ray.direction);
	return (ray);
}

t_ray	make_ray(t_camera *cam, uint32_t cx, uint32_t cy)
{
	if (cam->fov < 1.0)
		return make_ray_orthographic(cam, cx, cy);
	else if (cam->fov > 179.0)
		return make_ray_spherical(cam, cx, cy);
	else 
		return make_ray_perspective(cam, cx, cy);
}

t_ray	make_ray_to_point(double *origin, double *target){
	t_ray	ray;

	copy_vec(ray.origin, origin);
	sub_vec(ray.direction, target, origin);
	normalize(ray.direction);
	return (ray);
}
