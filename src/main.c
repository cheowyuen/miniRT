#include "minirt.h"

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
