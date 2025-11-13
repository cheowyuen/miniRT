#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "minirt.h"
#include "libft.h"

#define CAP_TOLERANCE 1e-4

void	assert_double(double actual, double expected, char *test_name)
{
	if (fabs(actual - expected) < CAP_TOLERANCE)
		printf("PASS: %s (Got: %.4f, Expected: %.4f)\n", test_name, actual, expected);
	else
		printf("FAIL: %s (Got: %.4f, Expected: %.4f)\n", test_name, actual, expected);
}

void test_dot_prod_cap_check(void)
{
	double	v[3]; // Cylinder Axis (V)
	double	p_minus_c[3]; // P - C (Vector from Center to Cap Hit Point)
	double	dot_product_result;
	
	printf("--- Testing Critical Dot Product for Cap Detection ---\n");

	// Setup: Cylinder Axis V (30 deg off X)
	v[0] = 0.866; v[1] = 0.0; v[2] = -0.500;
	
	// Setup: Vector from Center to Cap Hit Point (P - C)
	// For the Near Cap: P - C must point in the -V direction.
	// Expected length is half-height: H/2 = 5.0
	// P - C = -H/2 * V 
	
	p_minus_c[0] = -5.0 * v[0]; // -4.33
	p_minus_c[1] = -5.0 * v[1]; // 0.0
	p_minus_c[2] = -5.0 * v[2]; // +2.5
	
	// 1. Check the dot product: V . (P - C)
	// Expected: V . (-H/2 * V) = -H/2 * (V . V) = -5.0 * 1.0 = -5.0
	dot_product_result = dot_prod(v, p_minus_c);
	assert_double(dot_product_result, -5.0, "Dot Prod V . (P-C) at Cap");
	
	printf("------------------------------------------------------\n");
}


int main()
{
	// Note: This only tests the math functions on the required values.
	// We need to link src/vector.c to run dot_prod.
	test_dot_prod_cap_check();
	return (0);
}
