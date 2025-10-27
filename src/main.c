#include "minirt.h"
#include <assert.h>

void	ft_exit(void *param)
{
	t_app	*window;

	window = (t_app *)param;
	free_window(window);
	free_scene(window->scene);
	exit(window->exit_code);
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

static void run_scene(t_scene *scene)
{
	t_app	window;

	make_window(scene, &window);
	mlx_loop(window.mlx);
	window.exit_code = 0;
	ft_exit(&window);
}

int main(int argc, char **argv)
{
	t_scene	*scene;

	if (argc != 2)
	{
	    printf("Please give a filename as argument\n");
	    return (1);
	}
	scene = parse_scene(argv[1]);
	run_scene(scene);
	return (0);
}
