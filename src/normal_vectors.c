#include "minirt.h"

//check which surface of the cylinder point is on, and write the corresponding normal vector to result
static void	cylinder_normal(t_point *p)
{
	double	projection[3];
	double	center_to_point[3];
	double	temp[3];
	double	length;

	sub_vec(center_to_point, p->point, p->object->position);
	length = dot_prod(center_to_point, p->object->orientation); //assumes orientation is a unit vector, so no need to divide by o.o
	if (fabs(length) - (p->object->height / 2) < EPSILON) //on one of the ends
		mult_vec(p->normal, p->object->orientation, copysign(1.0, length)); //copysign returns the magnitude of first arg with sign of second, so this multiplies by 1.0 or -1.0
	else
	{//on the curved surface
		mult_vec(temp, p->object->position, length);
		add_vec(projection, p->object->position, temp);
		sub_vec(p->normal, center_to_point, projection);
	}
}

//store the normal vector at point p into p->normal
void make_normal_vector(t_point *p)
{
	if (p->object->type == PLANE)
		mult_vec(p->normal, p->object->orientation, 1); //TODO: figure out when it needs to be -1
	if (p->object->type == SPHERE)
	{
		sub_vec(p->normal, p->point, p->object->position);
		mult_vec(p->normal, p->normal, 1.0 / abs_vec(p->normal));
	}
	if (p->object->type == CYLINDER)
		cylinder_normal(p);
}
