#ifndef MINIRT_H
#define MINIRT_H

#include <MLX42.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define EPSILON 1e-9
#define HEIGHT 1000
#define WIDTH 1600

typedef enum e_type{
	SPHERE,
	PLANE,
	CYLINDER
} t_type;

typedef struct s_camera{
	double position[3];
	double direction[3];
	double fov;
} t_camera;

typedef struct s_light{
	double	position[3];
	bool	is_ambient;
	double	intensity;
	//int32_t color;
} t_light;

typedef struct s_object{
	t_type type;
	double position[3];
	double orientation[3];
	double var1;
	double var2;
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

typedef struct s_mlx_data{
	mlx_t		*mlx;
	mlx_image_t	*current;
} t_mlx_data;

//parsing.c
t_camera	*make_camera(char *str);
t_light		*make_light(char *str);
t_object	*make_object(char *str);
t_scene		*parse_scene(char *filename);
void 		free_scene(t_scene *scene);

//vector.c
void		add_vec(double *target, double *a, double *b);
void		sub_vec(double *target, double *a, double *b);
double		dot_prod(double *a, double *b);
void		mult_vec(double *target, double *vec, double scale);
double	 	abs_vec(double *vec);

//normal_vectors.c
void make_normal_vector(t_point *p, double *result);



#endif
