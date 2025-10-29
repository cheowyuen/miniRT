#ifndef MINIRT_H
#define MINIRT_H

#include <MLX42.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "libft.h"

#define EPSILON 1e-9
#define HEIGHT 1000
#define WIDTH 1000

typedef enum e_type{
	SPHERE,
	PLANE,
	CYLINDER
} t_type;

typedef struct s_camera{
	double	position[3];
	double	direction[3];
	double	fov;
	double	zero_coords[3];
	double	delta_x[3];
	double	delta_y[3];
} t_camera;

typedef struct s_light{
	double	position[3];
	bool	is_ambient;
	double	intensity;
	int32_t color;
} t_light;

typedef struct s_object{
	t_type type;
	double position[3];
	double orientation[3];
	double radius;
	double height;
	int32_t color;
} t_object;

typedef struct s_scene{
	t_camera	*camera;
	t_light		*ambient;
	t_light		**lights;
	size_t		num_lights;
	t_object	**objects;
	size_t		num_objects;
} t_scene;

typedef struct s_point{
	double		point[3];
	t_object	*object;
	double		normal[3];
} t_point;

typedef struct s_app{
	mlx_t		*mlx;
	mlx_image_t	*current;
	t_scene		*scene;
	int			exit_code;
} t_app;

typedef struct s_ray{
	double	origin[3];
	double	direction[3];
} t_ray;

//parsing.c
t_camera	*make_camera(char *str);
t_light		*make_light(char *str);
t_object	*make_object(char *str);
t_scene		*parse_scene(char *filename);
void 		free_scene(t_scene *scene);

//parsing_utils.c
void		cross_vec(double *target, double *a, double *b);
void		setup_geometry(t_camera *c);

//vector.c
void		add_vec(double *target, double *a, double *b);
void		sub_vec(double *target, double *a, double *b);
double		dot_prod(double *a, double *b);
void		mult_vec(double *target, double *vec, double scale);
double	 	abs_vec(double *vec);

//normal_vectors.c
void		make_normal_vector(t_point *p);

//lighting.c
int32_t		get_color(t_point *p, t_scene *s);
double		get_light_intensity(t_point *p, t_scene *s);

//main.c
void		ft_exit(void *param);

//window.c
void		free_window(t_app *window);
void		make_window(t_scene *scene, t_app *window);

//hooks.c
void		update_image(t_app *window);
void 		handle_keys(mlx_key_data_t keydata, void *param);
void		handle_resize(int32_t width, int32_t height, void* param);

//ray.c
int32_t		color_of_pixel(t_scene *scene, uint32_t cx, uint32_t cy);
void		normalize(double *d);

#endif
