#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "minirt.h"
#include "libft.h" // For ft_bzero

#define CAP_TOLERANCE 1e-4
#define TEST_RADIUS 5.0
#define TEST_HEIGHT 10.0
#define TEST_SAMPLES 20

// --- Utility Functions (Duplicated for standalone test file) ---

void	assert_vec(double *v, double x, double y, double z, char *test_name, int status)
{
	if (fabs(v[0] - x) < CAP_TOLERANCE && fabs(v[1] - y) < CAP_TOLERANCE && fabs(v[2] - z) < CAP_TOLERANCE)
	{
		if (status)
			printf("PASS: %s (Normal: %.3f, %.3f, %.3f)\n", test_name, v[0], v[1], v[2]);
	}
	else
		printf("FAIL: %s (Got: %.4f, %.4f, %.4f, Expected: %.4f, %.4f, %.4f)\n", 
            test_name, v[0], v[1], v[2], x, y, z);
}

// --- Test 1: Hit Point Generation ---

void test_cap_point_generation(void)
{
	t_object	cylinder;
	t_ray		ray;
	double		t_dist;
	double		hit_point[3];
	double		center_x;
	double		center_z;
	int			i;
	double		theta;
	
	printf("--- Testing Hit Point Generation Stability (20 Samples) ---\n");
	ft_bzero(&cylinder, sizeof(t_object));
	cylinder.type = CYLINDER;
	cylinder.position[0] = 0.0; cylinder.position[1] = 0.0; cylinder.position[2] = 20.0;
	cylinder.orientation[0] = 0.866; cylinder.orientation[1] = 0.0; cylinder.orientation[2] = -0.500;
	cylinder.radius = TEST_RADIUS;
	cylinder.height = TEST_HEIGHT;

	// Cap plane coordinates (calculated once, should be consistent)
	center_x = cylinder.position[0] - (TEST_HEIGHT / 2) * cylinder.orientation[0]; // -4.33
	center_z = cylinder.position[2] - (TEST_HEIGHT / 2) * cylinder.orientation[2]; // 22.50
	
	// FIX: Ensure t_dist is positive (Target X - Start X)
	t_dist = center_x - (-50.0); // -4.33 - (-50.0) = 45.67

	i = 0;
	while (i < TEST_SAMPLES)
	{
		theta = 2.0 * M_PI * i / TEST_SAMPLES;
		
		// Expected Radial Offsets (relative to the cap center C_near)
		// U (Up vector for the cap plane) is along Y, W (Right) is in the XZ plane
		double expected_y = TEST_RADIUS * cos(theta); 
		double expected_z_offset = TEST_RADIUS * sin(theta);
		
		// 1. Define Ray Origin
		ray.origin[0] = -50.0;
		ray.origin[1] = expected_y; 
		ray.origin[2] = center_z + expected_z_offset; 

		// 2. Ray Direction 
		ray.direction[0] = 1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
		
		// 3. Calculate Hit Point P (using raytracer's hit_points function)
		hit_points(hit_point, ray.origin, ray.direction, t_dist);

		// 4. Verification
		// Expected point must be on the plane X=center_x, with the same YZ offsets as the origin
		
		char test_name[50];
		sprintf(test_name, "Point Sample %02d", i + 1);
		
		assert_vec(hit_point, center_x, expected_y, ray.origin[2], test_name, 0);

		i++;
	}
}

// --- Test 2: Normal Stability (Using Raytracer Logic) ---

void test_cap_normal_stability(void)
{
	t_object	cylinder;
	t_point		p;
	t_ray		ray;
	double		expected_normal[3];
	double		hit_point[3];
	int			i;
	double		theta;
	double		t_dist;

	printf("\n--- Testing Cap Normal Stability (Raytracer Logic) ---\n");
	// Setup is identical to Test 1 to ensure point generation matches
	ft_bzero(&cylinder, sizeof(t_object));
	cylinder.type = CYLINDER;
	cylinder.position[0] = 0.0; cylinder.position[1] = 0.0; cylinder.position[2] = 20.0;
	cylinder.orientation[0] = 0.866; cylinder.orientation[1] = 0.0; cylinder.orientation[2] = -0.500;
	cylinder.radius = TEST_RADIUS;
	cylinder.height = TEST_HEIGHT;
	
	// Expected Normal (Near Cap: -V)
	expected_normal[0] = -0.866; expected_normal[1] = 0.0; expected_normal[2] = 0.500;
	
	double center_x = cylinder.position[0] - (TEST_HEIGHT / 2) * cylinder.orientation[0]; // -4.33
	double center_z = cylinder.position[2] - (TEST_HEIGHT / 2) * cylinder.orientation[2]; // 22.50
	
	// Ray origin starts at X=-50, aimed at cap plane X=-4.33
	t_dist = center_x - (-50.0); // 45.67

	i = 0;
	while (i < TEST_SAMPLES)
	{
		theta = 2.0 * M_PI * i / TEST_SAMPLES;
		
		// 1. Define Ray Origin
		ray.origin[0] = -50.0;
		ray.origin[1] = TEST_RADIUS * cos(theta); // Radial offset in Y
		ray.origin[2] = center_z + TEST_RADIUS * sin(theta); // Radial offset in Z
		
		// 2. Ray Direction (Always pointing +X, i.e., at (1, 0, 0))
		ray.direction[0] = 1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
		
		// 3. Calculate Hit Point P (using raytracer's hit_points function)
		hit_points(hit_point, ray.origin, ray.direction, t_dist);

		// 4. Test Normal at Hit Point P
		p.object = &cylinder;
		copy_vec(p.point, hit_point);
		// Crucially, use the updated make_normal_vector with increased tolerance or stable logic
		make_normal_vector(&p);

		char test_name[50];
		sprintf(test_name, "Sample %02d (t=%.2f)", i + 1, t_dist);
		
		assert_vec(p.normal, expected_normal[0], expected_normal[1], expected_normal[2], test_name, 0);

		i++;
	}
}

int main()
{
	test_cap_point_generation();
	test_cap_normal_stability();
	return (0);
}
