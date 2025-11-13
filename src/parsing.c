#include "minirt.h"

bool	is_normal(double *vec)
{
	double length;

	length = abs_vec(vec);
	if (length > 1.0)
	{
		if (vec[0] >= -1.0 && vec[0] <= 1.0 && vec[1] >= -1.0 && vec[1] <= 1.0\
 && vec[2] >= -1.0 && vec[2] <= 1.0 && length > 0.0001)
		{
			printf("Warning: components of vector in range [-1,1] but vector \
is not normalized, normalizing\n");
			normalize(vec);
		}
		else 
			return (false);
	}
	if (length < 0.0001)
		return (false);
	return (true);
}

void	check_requirements(t_scene *scene)
{
	int i;

	if (!scene->camera)
		parse_error("No camera defined", NULL, scene, NULL);
	if (!scene->ambient)
		parse_error("No ambient light defined", NULL, scene, NULL);
	if (scene->num_objects == 0)
		parse_error("No objects defined", NULL, scene, NULL);
	if (!is_normal(scene->camera->direction))
		parse_error("Camera direction not normalized", NULL, scene, NULL);
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		if ((scene->objects[i]->type == PLANE || scene->objects[i]->type == \
CYLINDER) && !is_normal(scene->objects[i]->orientation))
			parse_error("Object orientation not normalized", NULL, scene, NULL);
	}
}

t_camera	*make_camera(char *str, t_scene *scene)
{
	t_camera	*camera;
	int			i;
	ssize_t		temp;

	i = 1;	
	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		parse_error("malloc error: ", strerror(errno), scene, NULL);
	while (str[i] == ' ')
		++i;
	temp = parse_triplet(camera->position, str + i);
	if (temp == -1)
		parse_error("error parsing position on line: ", str, scene, camera);
	i += temp;
	while (str[i] == ' ')
		++i;
	temp = parse_triplet(camera->direction, str + i);
	if (temp == -1)
		parse_error("error parsing direction on line: ", str, scene, camera);
	i += temp;
	while (str[i] == ' ')
		++i;
	camera->fov = ft_atoi(str + i); 
	if (camera->fov < 0 || camera->fov > 180)
		parse_error("error parsing fov on line: ", str, scene, camera);
	setup_geometry(camera, true, WIDTH, HEIGHT);
	return (camera);
}

t_light	*make_light(char *str, t_scene *scene, bool ambient)
{
	t_light *light;
	int		i;
	ssize_t	temp;

	light = ft_calloc(1, sizeof(t_light));
	if (!light)
		parse_error("malloc error: ", strerror(errno), scene, NULL);
	i = 1;
	while (str[i] == ' ')
		++i;
	if (!ambient)
	{
		temp = parse_triplet(light->position, str + i);
		if (temp == -1)
			parse_error("error parsing position on line: ", str, scene, light);
		i += temp;
	}
 	light->intensity = ft_atod(str + i, &i, ' ');
	if (i <= 0 || light->intensity < 0 || light->intensity > 1)
		parse_error("error parsing intensity on line: ", str, scene, light);
	if (parse_color(&light->color, str + i) == -1)
		parse_error("error parsing color on line: ", str, scene, light);
	light->is_ambient = ambient;
	return (light);
}

void	parse_plane(char *str, t_object *object, t_scene *scene)
{
	ssize_t	temp;
	int		i;

	i = 1;
	while (str[i] == ' ')
		++i;
	temp = parse_triplet(object->position, str + i);
	if (temp == -1)
		parse_error("error parsing position on line: ", str, scene, object);
	i += temp;
	temp = parse_triplet(object->orientation, str + i);
	if (temp == -1)
		parse_error("error parsing direction on line: ", str, scene, object);
	i += temp;
	temp = parse_color(&object->color, str + i);
	if (temp == -1)
		parse_error("error parsing color on line: ", str, scene, object);
	object->type = PLANE;
}

void	parse_sphere(char *str, t_object *object, t_scene *scene)
{
	ssize_t	temp;
	int		i;

	i = 1;
	temp = parse_triplet(object->position, str + i);
	if (temp == -1)
		parse_error("error parsing position on line: ", str, scene, object);
	i += temp;
	object->radius = ft_atod(str + i, &i, ' ');
	if (i == -1 || object->radius <= 0)
		parse_error("error parsing radius on line: ", str, scene, object);
	temp = parse_color(&object->color, str + i);
	if (temp == -1)
		parse_error("error parsing color on line: ", str, scene, object);
	object->type = SPHERE;
}

void	parse_cylinder(char *str, t_object *object, t_scene *scene)
{
	ssize_t	temp;
	int		i;

	i = 1;
	temp = parse_triplet(object->position, str + i);
	if (temp == -1)
		parse_error("error parsing position on line: ", str, scene, object);
	i += temp;
	temp = parse_triplet(object->orientation, str + i);
	if (temp == -1)
		parse_error("error parsing direction on line: ", str, scene, object);
	i += temp;
	object->radius = ft_atod(str + i, &i, ' ');
	if (i == -1 || object->radius <= 0)
		parse_error("error parsing radius on line: ", str, scene, object);
	object->height = ft_atod(str + i, &i, ' ');
	if (i == -1 || object->height <= 0)
		parse_error("error parsing height on line: ", str, scene, object);
	temp = parse_color(&object->color, str + i);
	if (temp == -1)
		parse_error("error parsing color on line: ", str, scene, object);
	object->type = CYLINDER;
}

//makes an object of the specified type and adds it to the array
void	make_object(char *str, t_scene *scene)
{
	t_object	*object;
	t_object	**array;
	int			i;
	double		right[3];

	object = ft_calloc(1, sizeof(t_object));
	if (!object)
		parse_error("malloc error: ", strerror(errno), scene, NULL);
	i = 2;
	if (!ft_strncmp(str, "pl", 2))
		parse_plane(str + i, object, scene);
	if (!ft_strncmp(str, "sp", 2))
		parse_sphere(str + i, object, scene);
	if (!ft_strncmp(str, "cy", 2))
		parse_cylinder(str + i, object, scene);
	array = ft_calloc(scene->num_objects + 1, sizeof(void *));
	if (!array)
		parse_error("malloc error: ", strerror(errno), scene, object);
	++(scene->num_objects);
	i = -1;
	while (++i < (int)scene->num_objects - 1)
		array[i] = scene->objects[i];
	array[i] = object;
	free(scene->objects);
	scene->objects = array;
	cal_vec(object->orientation, object->up, right, true);
}

void	check_and_add_line(char *line, t_scene *s)
{
	if (!*line)
		return ;
	if (*line == 'L')
	{
		if (s->light)
			parse_error("Second definition of light", NULL, s, NULL);
		s->light = make_light(line, s, false);
	}
	else if (*line == 'A')
	{
		if (s->ambient)
			parse_error("Second definition of ambient", NULL, s, NULL);
		s->ambient = make_light(line, s, true);
	}
	else if (*line == 'C')
	{
		if (s->camera)
			parse_error("Second definition of camera", NULL, s, NULL);
		s->camera = make_camera(line, s);
	}
	else if (!ft_strncmp(line, "pl", 2) || !ft_strncmp(line, "cy", 2) ||\
!ft_strncmp(line, "sp", 2))
		make_object(line, s);
	else
		parse_error("Unrecognised format of line: ", line, s, NULL);
}

void	parse_error(char *msg, char *arg, void *scene, void *object)
{
	printf("Error\n");
	printf("%s", msg);
	if (arg)
		printf("%s", arg);
	printf("\n");
	if(((t_scene *)scene)->fd > 0)
		close(((t_scene *)scene)->fd); 
	free(((t_scene *)scene)->line);
	free(scene);
	free(object);
	exit(1);
}

t_scene	*parse_scene(char *filename)
{
	t_scene	*s;

	s = malloc(sizeof(t_scene));
	if (!s)
		return (NULL);
	ft_memset(s, 0, sizeof(t_scene));
	if (ft_strncmp((filename + ft_strlen(filename) - 3), ".rt", 3))
		parse_error("Invalid file extension: ", filename, s, NULL);
	s->fd = open(filename, O_RDONLY);
	if (s->fd == -1)
		parse_error("Error opening file: ", filename, s, NULL);
	s->line = get_next_line(s->fd);
	while (s->line != NULL && ft_strlen(s->line) > 0)
	{
		if (s->line[ft_strlen(s->line) - 1] == '\n')
			s->line[ft_strlen(s->line) - 1] = '\0';
		check_and_add_line(s->line, s);
		free(s->line);
		s->line = get_next_line(s->fd);
	}
	check_requirements(s);
	s->selected_type = SELECT_OBJECT;
	s->selected_index = 0;
	return (s);
}
