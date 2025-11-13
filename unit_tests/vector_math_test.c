#include <stdio.h>
#include <math.h>
#include "minirt.h" // Assuming minirt.h contains the function prototypes

void	assert_vec(double *v, double x, double y, double z, char *test_name)
{
	if (fabs(v[0] - x) < EPSILON && fabs(v[1] - y) < EPSILON && fabs(v[2] - z) < EPSILON)
		printf("PASS: %s\n", test_name);
	else
		printf("FAIL: %s (Got: %.2f, %.2f, %.2f, Expected: %.2f, %.2f, %.2f)\n", 
            test_name, v[0], v[1], v[2], x, y, z);
}

void test_cross_vec(void)
{
	double a[3] = {1.0, 0.0, 0.0};
	double b[3] = {0.0, 1.0, 0.0};
	double result[3];
	
	cross_vec(result, a, b);
	assert_vec(result, 0.0, 0.0, 1.0, "Cross Product (i x j)");
}

void test_normalize(void)
{
    double d[3] = {0.0, 5.0, 0.0};
    normalize(d);
    assert_vec(d, 0.0, 1.0, 0.0, "Normalize (0, 5, 0)");
}

void test_dot_prod(void)
{
    double a[3] = {1.0, 2.0, 3.0};
    double b[3] = {4.0, 5.0, 6.0};
    double result = dot_prod(a, b);
    
    printf("\n--- Testing dot_prod ---\n");
    if (fabs(result - 32.0) < EPSILON)
        printf("PASS: General Dot Product (1,2,3) . (4,5,6) == 32.0\n");
    else
        printf("FAIL: General Dot Product (Got: %.2f, Expected: 32.0)\n", result);
}
// ... other math tests ...

int main(void)
{
    test_cross_vec();
    test_normalize();
	test_dot_prod();
    return (0);
}
