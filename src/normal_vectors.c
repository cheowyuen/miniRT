#include "minirt.h"

//store the normal vector at point p into result, which must have size 3
void make_normal_vector(t_point *p, double *result)
{
	if (p->type == PLANE)
		mult_vec(result, p->object->orientation, 1); //TODO: figure out when it needs to be -1
	if (p->type == SPHERE)
	{
		sub_vec(result, p->point, p->object->position);
		mult_vec(result, result, 1.0 / abs_vec(result));
	}
	if (p->type == CYLINDER)
		cylinder_normal(p, result);
}

//check which surface of the cylinder point is on, and return the corresponding normal vector
void	cylinder_normal(t_point *p, double *result)
{
	double	projection[3];
	double	center_to_point[3];
	double	temp[3];
	double	length;

	vec_sub(center_to_point, p->point, p->object->position);
	length = dot_prod(center_to_point, p->object->orientation); //assumes orientation is a unit vector, so no need to divide by o.o
	if (fabs(length) - (p->object->var1 / 2) < EPSILON) //on one of the ends
		vec_mult(result, p->object->orientation, copysign(1.0, length)); //copysign returns the magnitude of first arg with sign of second
	else
	{//on the curved surface
		vec_mult(temp, p->object->position, length);
		vec_add(projection, p->object->position, temp);
		vec_sub(result, center_to_point, projection);
	}
}
