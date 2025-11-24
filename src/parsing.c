#include "minirt.h"

static int	skip_spaces(char *str, int i)
{
	while (str[i] == ' ')
		++i;
	return (i);
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
	i = skip_spaces(str, i);
	temp = parse_triplet(camera->position, str + i);
	if (temp == -1)
		parse_error("error parsing position on line: ", str, scene, camera);
	i += temp;
	i = skip_spaces(str, i);
	temp = parse_triplet(camera->direction, str + i);
	if (temp == -1)
		parse_error("error parsing direction on line: ", str, scene, camera);
	i += temp;
	i = skip_spaces(str, i);
	camera->fov = ft_atoi(str + i);
	if (camera->fov < 0 || camera->fov > 180)
		parse_error("error parsing fov on line: ", str, scene, camera);
	setup_geometry(camera, WIDTH, HEIGHT);
	return (camera);
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
	if (((t_scene *)scene)->fd > 0)
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
	check_in_objects(s);
	s->selected_type = SELECT_OBJECT;
	s->selected_index = 0;
	return (s);
}
