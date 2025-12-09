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

#define EPSILON 1e-3
#define	LINEAR 0.01
#define	QUAD 0.001
#define HEIGHT 1000
#define WIDTH 1000
#define REDRAW_INTERVAL 3.0
#define ROTATION_ANGLE 0.05

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
	double	zero[3];
	double	delta_x[3];
	double	delta_y[3];
	double	up[3];
	double	right[3];
	double	h;
	double	w;
	double	inverse_w;
	double	inverse_h;
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
	double		last_render_time;
	bool		resize_pending;
} t_app;

typedef struct s_ray{
	double	origin[3];
	double	direction[3];
} t_ray;

typedef struct s_hit{
	t_object	*obj;
	double		t;
} t_hit;

typedef struct s_vec{
	double	oc[3];
	double	hit[3];
	double	vec[3];
	double	perp[3];
} t_vec;

//parsing.c
t_scene		*parse_scene(char *filename);
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
void		cal_right(double *right, double *default_up, double *direction);
void		cal_vec(double *direction, double *up, double *right, bool is_init);

//camera.c
void		setup_geometry(t_camera *c, int32_t width, int32_t height);

//vector.c
void		add_vec(double *target, double *a, double *b);
void		sub_vec(double *target, double *a, double *b);
double		dot_prod(double *a, double *b);
void		mult_vec(double *target, double *vec, double scale);
double	 	abs_vec(double *vec);

//vector_utils.c
void		cross_vec(double *target, double *a, double *b);
void		normalize(double *d);
void		copy_vec(double *target, double *vec);

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
void		print_selected_element(t_scene *s);

//hooks.c
void		update_image(t_app *window);
void 		handle_keys(mlx_key_data_t keydata, void *param);

//ray.c
int32_t		color_of_pixel(t_scene *scene, uint32_t cx, uint32_t cy);
bool		hit_object(double *dir, double *oc, double r, double *t);
bool		hit_plane(t_ray ray, t_object *plane, double *t);

//ray_utils.c
void		make_point(t_point *p, double *position, t_object *object);
void		hit_points(double *hit, double *origin, double *direction, double t);
void		proj_perpendicular(double *vec, double *axis, double *result);
void 		add_scaled_vecs(double pos[3], t_camera *camera, int cx, int cy);
void		make_coeffs(double *coeffs, double **vectors, double radius);


//ray_makers.c
t_ray		make_ray(t_camera *c, uint32_t cx, uint32_t cy);
t_ray		make_ray_to_point(double *origin, double *target);

//cylinder.c
bool		hit_cylinder(t_ray ray, t_object *cylinder, double *t);
bool		hit_caps(t_ray ray, t_object *cylinder, double *t);
bool		cal_t(double a, double b, double discriminant, double *t);

//translation.c
void		move_camera(keys_t key, t_app *window);

//resize.c
void 		resize_obj(keys_t key, t_app *window);
void		handle_resize(int32_t width, int32_t height, void* param);

//rotation.c
void		pan_camera(keys_t key, t_app *window);

//colors.c
void 		blend_colors(double *target, int32_t a, int32_t b);
int32_t 	add_colors(double *color_a, double *color_b);

//validate_scene.c
bool		check_in_objects(t_scene *s, bool close);
void		check_requirements(t_scene *scene);

#endif
