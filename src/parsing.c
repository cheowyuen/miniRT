#include "minirt.h"

t_camera	*make_camera(char *str, t_scene *scene)
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

t_light	*make_light(char *str, t_scene *scene, bool ambient)
{
	t_light *light;
	int		i;

	light = malloc(sizeof(t_light));
	if (!light)
		parse_error("malloc error: ", strerror(errno), scene);
	ft_memset(light, 0, sizeof(t_light));
	i = 0;
	light->position[0] = -40.0;
	light->position[1] = 50.0;
	light->position[2] = 0.0;
	light->intensity = 0.2;
	light->is_ambient = ambient;
	return (light);
}

t_object	*make_object(char *str, t_scene *scene)
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

void	check_and_add_line(char *line, t_scene *s)
{
	if (*line == '\0')
		return ;
	if (*line == 'L')
		make_light(line, s, false);
	if (*line == 'A')
		make_light(line, s, true);
	if (*line == 'C')
		make_camera
}

void	parse_error(char *msg, char *arg, void *scene)
{
	printf("Error\n");
	printf(msg);
	if (arg)
		printf(arg);
	printf("\n");
	free(scene);
	exit(1);
}

t_scene	*parse_scene(char *filename)
{
	t_scene	*s;
	int		fd;
	char 	*line;

	s = malloc(sizeof(t_scene));
	if (!s)
		return (NULL);
	ft_memset(s, 0, sizeof(t_scene));
	if (!ft_strcmp((filename + ft_strlen(filename) - 3), ".rt")
		parse_error("Invalid filename: ", filename, s);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		parse_error("Error opening file: ", filename, s);
	line = get_next_line(fd);
	while (line != NULL)
	{
		check_and_add_line(line, s);
		line = get_next_line(fd);
	}
	check_requirements(s);
	return (s);
}
