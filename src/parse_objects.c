/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nraatika <nraatika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:45:45 by nraatika          #+#    #+#             */
/*   Updated: 2025/12/18 17:02:46 by nraatika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_plane(char *str, t_object *object, t_scene *scene)
{
	ssize_t	temp;
	int		i;

	i = 1;
	while (str[i] == ' ' || str[i] == '\t')
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
	object->radius = ft_atod(str + i, &i, ' ') / 2.0;
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
	object->radius = ft_atod(str + i, &i, ' ') / 2.0;
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
	if (!ft_strncmp(str, "pl", 2))
		parse_plane(str + 2, object, scene);
	if (!ft_strncmp(str, "sp", 2))
		parse_sphere(str + 2, object, scene);
	if (!ft_strncmp(str, "cy", 2))
		parse_cylinder(str + 2, object, scene);
	array = ft_calloc(++(scene->num_objects), sizeof(void *));
	if (!array)
		parse_error("malloc error: ", strerror(errno), scene, object);
	i = -1;
	while (++i < (int)scene->num_objects - 1)
		array[i] = scene->objects[i];
	array[i] = object;
	free(scene->objects);
	scene->objects = array;
	cal_vec(object->orientation, object->up, right, true);
}

t_light	*make_light(char *str, t_scene *scene, bool ambient)
{
	t_light	*light;
	int		i;
	ssize_t	temp;

	light = ft_calloc(1, sizeof(t_light));
	if (!light)
		parse_error("malloc error: ", strerror(errno), scene, NULL);
	i = 1;
	while (str[i] == ' ' || str[i] == '\t')
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
