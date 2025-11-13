#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include "minirt.h" // Now relying on your main header for struct definitions

#define ASSERT_EPSILON 1e-5 // Tolerance for floating-point comparisons

// --- Assertion Helpers (Redefining if not in minirt.h) ---

/**
 * @brief Checks if a hit result matches the expected outcome.
 */
void assert_hit_result(double actual, double expected, char *test_name, bool is_hit)
{
    // [Implementation omitted for brevity, assume previous correct implementation]
    if (!is_hit)
    {
        if (expected > 0.0)
            printf("FAIL: %s (Miss, Expected Hit at t=%.4f)\n", test_name, expected);
        else
            printf("PASS: %s (Miss, Expected Miss)\n", test_name);
        return;
    }
    if (fabs(actual - expected) < ASSERT_EPSILON)
        printf("PASS: %s (t=%.4f)\n", test_name, actual);
    else
        printf("FAIL: %s (t=%.4f, Expected: %.4f)\n", test_name, actual, expected);
}

/**
 * @brief Checks if a vector matches the expected XYZ components.
 */
void assert_vec(double *v, double x, double y, double z, char *test_name)
{
    if (fabs(v[0] - x) < ASSERT_EPSILON && fabs(v[1] - y) < ASSERT_EPSILON && fabs(v[2] - z) < ASSERT_EPSILON)
        printf("PASS: %s (Normal: %.2f, %.2f, %.2f)\n", test_name, v[0], v[1], v[2]);
    else
        printf("FAIL: %s (Got: %.4f, %.4f, %.4f, Expected: %.2f, %.2f, %.2f)\n", 
            test_name, v[0], v[1], v[2], x, y, z);
}

// --- Normal Vector Tests ---

// Based on make_normal_vector (src/normal_vectors.c)
void test_cylinder_normals(void)
{
    t_object cyl;
    t_point p;
    
    printf("\n--- Testing Cylinder Normals (make_normal_vector) ---\n");

    // Setup Cylinder: Pos (0, 0, 10), Dir (0, 1, 0) [Vertical], R=2, H=4
    ft_bzero(&cyl, sizeof(cyl));
    ft_bzero(&p, sizeof(p));
    cyl.position[2] = 10.0;
    cyl.orientation[1] = 1.0; // Vertical cylinder (Axis Y)
    cyl.radius = 2.0;
    cyl.height = 4.0;
	cyl.type = CYLINDER;
    p.object = &cyl;

    // Test 1: Side Normal (Hit Point: (2, 0, 10))
    // Expected: Normal is purely radial (1, 0, 0)
    p.point[0] = 2.0; p.point[1] = 0.0; p.point[2] = 10.0;
    make_normal_vector(&p);
    assert_vec(p.normal, 1.0, 0.0, 0.0, "NORMAL 1: Side Hit (Radial)");

    // Test 2: Top Cap Normal (Hit Point: (0, 2, 10))
    // Top cap center is (0, 2, 10). Length to point is 2.0. Length is (H/2) = 2.0.
    // Length check: fabs(length) - (H/2) < EPSILON
    // The point is exactly on the cap's center (Y=2), which is where the side meets the cap.
    p.point[0] = 0.0; p.point[1] = 2.0; p.point[2] = 10.0;
    make_normal_vector(&p);
    // Expected: Matches orientation vector (0, 1, 0)
    assert_vec(p.normal, 0.0, 1.0, 0.0, "NORMAL 2: Top Cap Center");

    // Test 3: Bottom Cap Normal (Hit Point: (0, -2, 10))
    // Bottom cap center is (0, -2, 10).
    p.point[0] = 0.0; p.point[1] = -2.0; p.point[2] = 10.0;
    make_normal_vector(&p);
    // Expected: Matches negative orientation vector (0, -1, 0)
    assert_vec(p.normal, 0.0, -1.0, 0.0, "NORMAL 3: Bottom Cap Center");

    // Test 4: Hit Point just *below* the Top Cap (Y=1.99)
    // This is a side hit, but very close to the edge.
    p.point[0] = 2.0; p.point[1] = 1.99; p.point[2] = 10.0;
    make_normal_vector(&p);
    // Expected: Radial normal (1, 0, 0)
    assert_vec(p.normal, 1.0, 0.0, 0.0, "NORMAL 4: Near Top Side");
    
    // Test 5: Hit Point just *on* the Top Cap surface (Y=2.0, R=1.0)
    // Cap is at Y=2.0. Hit at (1, 2, 10)
    p.point[0] = 1.0; p.point[1] = 2.0; p.point[2] = 10.0;
    make_normal_vector(&p);
    // Expected: (0, 1, 0)
    assert_vec(p.normal, 0.0, 1.0, 0.0, "NORMAL 5: On Top Cap Surface");
}

// ... existing intersection tests ...

int main(void)
{
//    test_sphere_intersection();
//    test_plane_intersection();
//    test_cylinder_intersection();
    test_cylinder_normals(); // Call the new test suite
    
    return (0);
}
