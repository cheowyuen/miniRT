#ifndef MINIRT_H
#define MINIRT_H

#include <MLX42.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libft.h"

#define EPSILON 1e-9
#define	LINEAR 0.01f
#define	QUAD 0.001f
#define HEIGHT 1000
#define WIDTH 1000

typedef enum e_type{
	SPHERE,
	PLANE,
	CYLINDER
} t_type;

typedef enum e_selected_type{
	SELECT_OBJECT,
	SELECT_LIGHT,
	SELECT_CAMERA,
} t_selected_type;

typedef struct s_camera{
	double	position[3];
	double	direction[3];
	double	fov;
	double	zero_coords[3];
	double	delta_x[3];
	double	delta_y[3];
	double	up[3];
} t_camera;

typedef struct s_light{
	double	position[3];
	bool	is_ambient;
	double	intensity;
	int32_t color;
} t_light;

typedef struct s_object{
	t_type 	type;
	double 	position[3];
	double 	orientation[3];
	double 	radius;
	double 	height;
	int32_t color;
	double	up[3];
} t_object;

typedef struct s_scene{
	t_camera		*camera;
	t_light			*ambient;
	t_light			*light;
	t_object		**objects;
	size_t			num_objects;
	char			*line;
	int				fd;
	t_selected_type	selected_type;
	size_t			selected_index;
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
	int32_t		width;
	int32_t		height;
} t_app;

typedef struct s_ray{
	double	origin[3];
	double	direction[3];
} t_ray;

//parsing.c
t_scene		*parse_scene(char *filename);
void		check_requirements(t_scene *scene);
t_camera	*make_camera(char *str, t_scene *scene);
t_light		*make_light(char *str, t_scene *scene, bool ambient);
void		parse_plane(char *str, t_object *object, t_scene *scene);
void		parse_sphere(char *str, t_object *object, t_scene *scene);
void		parse_cylinder(char *str, t_object *object, t_scene *scene);
void		make_object(char *str, t_scene *scene);
void		check_and_add_line(char *line, t_scene *s);
void		parse_error(char *msg, char *arg, void *scene, void *object);

//parse_utils.c
double 		ft_atod(char *line, int *read, char delimiter);
ssize_t		parse_color(int32_t *color, char *str);
ssize_t		parse_triplet(double *target, char *str);


//parsing_utils.c
void		cross_vec(double *target, double *a, double *b);
void		normalize(double *d);
void		setup_geometry(t_camera *c, bool is_init, int32_t width, int32_t height);
void		cal_right(double *right, double *default_up, double *direction);
void		copy_vec(double *target, double *vec);
void		setup_geometry_obj(t_object *obj, bool is_init);

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
void 		free_scene(t_scene *scene);

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
bool		hit_caps(t_ray ray, t_object *cylinder, double *t);
bool		hit_object(double *dir, double *oc, double r, double *t);
bool		hit_cylinder(t_ray ray, t_object *cylinder, double *t);
bool		hit_plane(t_ray ray, t_object *plane, double *t);
t_ray		make_ray(double *origin, double pixel_pos[3]);

#endif
