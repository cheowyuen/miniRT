#include "minirt.h"
#include <assert.h>

static int vectors_equal(double *v1, double *v2, double epsilon) {
    for (int i = 0; i < 3; i++) {
        if (fabs(v1[i] - v2[i]) > epsilon) {
            return 0;
        }
    }
    return 1;
}

static void	test_sphere_normal()
{
	t_object sphere = {SPHERE, {0, 0, 0}, {0, 0, 0}, 3.0, 0.0, 0};
    t_point point_on_sphere = {{3, 0, 0}, &sphere, {0, 0, 0}};
    double expected_normal[3] = {1, 0, 0};

    make_normal_vector(&point_on_sphere);
    assert(vectors_equal(point_on_sphere.normal, expected_normal, 1e-6) && "Sphere normal test failed");
	printf("got normal %f %f %f\n", point_on_sphere.normal[0], point_on_sphere.normal[1], point_on_sphere.normal[2]);
}

static void run_scene(t_scene *scene)
{
	t_mlx_data	window;

	// (void)scene;
	printf("Imagine a beatiful sphere\n");
	window.mlx = mlx_init(HEIGHT, WIDTH, "miniRT", false);
	window.current = mlx_new_image(window.mlx, HEIGHT, WIDTH);
	for_each_pixel(scene);
	//make_viewport(scene->camera);
//	update_image(scene, window.current);
}

int main(int argc, char **argv)
{
	test_sphere_normal();
	// return (1);

	t_scene	*scene;

	if (argc != 2)
	{
	    printf("Please give a filename as argument\n");
	    // return (1);
	}
	scene = parse_scene(argv[1]);
	run_scene(scene);
	free_scene(scene);
	return (0);
}
