#ifndef MINIRT_H
#define MINIRT_H

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

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
	t_camera *camera;
	t_light *ambient;
	t_light **lights;
	size_t num_lights;
	t_object **objects;
	size_t num_objects;
} t_scene;

t_camera	*make_camera(char *str);
t_light		*make_light(char *str);
t_object	*make_object(char *str);
t_scene		*parse_scene(char *filename);
void 		free_scene(t_scene *scene);

#endif
