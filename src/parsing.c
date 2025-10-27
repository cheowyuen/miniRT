#include "minirt.h"

t_camera	*make_camera(char *str)
{
	t_camera	*camera;

	(void)str;
	camera = malloc(sizeof(t_camera));
	camera->position[0] = -50.0;
	camera->position[1] = 0.0;
	camera->position[2] = 20.0;
	camera->direction[0] = 0.0;
	camera->direction[1] = 0.0;
	camera->direction[2] = 1.0;
	camera->fov = 70;
	return (camera);
}

t_light	*make_light(char *str)
{
	t_light *light;

	(void)str;
	light = malloc(sizeof(t_light));
	light->position[0] = -40.0;
	light->position[1] = 50.0;
	light->position[2] = 0.0;
	light->intensity = 0.2;
	light->is_ambient = false;
	return (light);
}

t_object	*make_object(char *str)
{
	t_object *object;

	(void)str;
	object = malloc(sizeof(t_object));
	object->type = PLANE;
	object->position[0] = 0.0;
	object->position[1] = 0.0;
	object->position[2] = 0.0;
	object->orientation[0] = 0.0;
	object->orientation[1] = 1.0;
	object->orientation[2] = 0.0;
	object->color = (225 << 8 | 255);
	return (object);
}

t_scene	*parse_scene(char *filename)
{
	t_scene	*s;

	(void)filename;
	s = malloc(sizeof(t_scene));
	s->camera = make_camera("input_line");
	s->ambient = make_light("input_line");
	s->ambient->is_ambient = true;
	s->num_lights = 1;
	s->lights = malloc(sizeof(t_light *) * s->num_lights);
	s->num_objects = 3;
	s->objects = malloc(sizeof(t_object *) * s->num_objects);
	s->lights[0] = make_light("input line");
	s->objects[0] = make_object("input line");
	s->objects[1] = make_object("input line");
	s->objects[2] = make_object("input line");
	s->objects[1]->type = SPHERE;
	s->objects[1]->position[0] = -50;
	s->objects[1]->position[2] = 50;
	s->objects[1]->radius = 5;
	s->objects[0]->type = PLANE;
	s->objects[0]->position[2] = 30;
	s->objects[0]->orientation[1] = 0.0;
	s->objects[0]->orientation[2] = 1.0;
	s->objects[2]->type = CYLINDER;
	s->objects[2]->position[0] = -50.0;
	s->objects[2]->position[2] = 50.0;
	s->objects[2]->orientation[1] = 1.0;
	s->objects[2]->radius = 2.0;
	s->objects[2]->height = 6.0;
	return (s);
}

void free_scene(t_scene *scene)
{
	int i;

	free(scene->camera);
	free(scene->ambient);
	i = -1;
	while (++i < (int)scene->num_lights)
		free(scene->lights[i]);
	i = -1;
	while (++i < (int)scene->num_objects)
		free(scene->objects[i]);
	free(scene);
}
