#include "minirt.h"

static void run_scene(t_scene *scene)
{
	t_mlx_data	window;

	(void)scene;
	printf("Imagine a beatiful sphere\n");
	window.mlx = mlx_init(HEIGHT, WIDTH, "miniRT", false);
	window.current = mlx_new_image(window.mlx, HEIGHT, WIDTH);
	//make_viewport(scene->camera);
//	update_image(scene, window.current);
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
	free_scene(scene);
	return (0);
}
