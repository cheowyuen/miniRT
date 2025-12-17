/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:43:52 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/17 09:44:55 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_exit(void *param)
{
	t_app	*window;

	window = (t_app *)param;
	free_window(window);
	free_scene(window->scene);
	exit(window->exit_code);
}

void	free_scene(t_scene *scene)
{
	int	i;

	free(scene->camera);
	free(scene->ambient);
	free(scene->light);
	i = -1;
	while (++i < (int)scene->num_objects)
		free(scene->objects[i]);
	free(scene->objects);
	free(scene);
}

static void	run_scene(t_scene *scene)
{
	t_app	window;

	ft_bzero(&window, sizeof(t_app));
	make_window(scene, &window);
	update_image(&window);
	print_selected_element(window.scene);
	mlx_loop(window.mlx);
	window.exit_code = 0;
	ft_exit(&window);
}

int	main(int argc, char **argv)
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
