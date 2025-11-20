#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

/* --- Mocks for your project structs --- */
typedef struct s_ray {
    double origin[3];
    double direction[3];
} t_ray;

typedef struct s_camera {
    double position[3];
    double direction[3];
    double right[3];
    double up[3];
    double w;
    double h;
    double inverse_w;
    double inverse_h;
} t_camera;

/* --- Helper Mocks --- */
void copy_vec(double *dest, double *src) {
    dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2];
}
double abs_vec(double *v) {
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
void normalize(double *v) {
    double l = abs_vec(v);
    if (l > 0) { v[0]/=l; v[1]/=l; v[2]/=l; }
}

/* --- The Function Under Test (from src/ray_makers.c) --- */
t_ray	make_ray_spherical(t_camera *cam, uint32_t cx, uint32_t cy)
{
	t_ray	ray;
	double	angles[2]; // theta, phi
	double	xy[2];
	double	local_dir[3];
	
	copy_vec(ray.origin, cam->position);
	xy[0] = (double)cx * cam->inverse_w - 0.5;
	xy[1] = (double)cy * cam->inverse_h - 0.5;
	
	// Mapped angles based on your code
	angles[0] = xy[0] * M_PI; 
	angles[1] = xy[1] * M_PI * cam->h * cam->inverse_w;

	// The problematic conversion
	local_dir[0] = sin(angles[0]) * cos(angles[1]);
	local_dir[1] = -sin(angles[1]);
	local_dir[2] = cos(angles[0]) * cos(angles[1]);

	int i = -1;
	while (++i < 3)
	{
		// Transforms local vector to camera basis (Right, Up, Forward)
		ray.direction[i] = (local_dir[0] * cam->right[i]) + \
						   (local_dir[1] * cam->up[i]) + \
						   (local_dir[2] * cam->direction[i]);
	}
	normalize(ray.direction);
	return (ray);
}

/* --- Test Main --- */
int main() {
	t_camera cam;
	
	// 1. Setup Camera based on fov_180.rt: C 0,0,30 0,0,-1 180
	double width = 800;
	double height = 800; // Square aspect ratio for simplicity in test
	
	cam.position[0] = 0; cam.position[1] = 0; cam.position[2] = 30;
	
	// Looking towards -Z
	cam.direction[0] = 0; cam.direction[1] = 0; cam.direction[2] = -1;
	
	// Standard Up vector (Y)
	cam.up[0] = 0; cam.up[1] = 1; cam.up[2] = 0;
	
	// Right vector (X)
	cam.right[0] = 1; cam.right[1] = 0; cam.right[2] = 0;
	
	cam.w = width;
	cam.h = height;
	cam.inverse_w = 1.0 / width;
	cam.inverse_h = 1.0 / height;

	printf("=== Spherical Camera Debug Test ===\n");
	printf("Camera Config: Pos(0,0,30) Dir(0,0,-1) Up(0,1,0)\n");
	printf("Expectation: Center pixel should point (0,0,-1)\n\n");

	// 2. Test Center Pixel
	// At center: cx = 400, cy = 400 -> xy = {0, 0} -> angles = {0, 0}
	t_ray ray = make_ray_spherical(&cam, width/2, height/2);
	
	printf("Pixel (%d, %d) [Center]:\n", (int)(width/2), (int)(height/2));
	printf("  Result Ray Dir: %.2f, %.2f, %.2f\n", 
		ray.direction[0], ray.direction[1], ray.direction[2]);
	
	if (fabs(ray.direction[1] - 1.0) < 0.01) {
		printf("  [FAIL] Ray is pointing UP (0,1,0) instead of FORWARD (0,0,-1).\n");
		printf("         Reason: In local_dir, angles[1]=0 makes cos(angles[1])=1.\n");
		printf("         This maps to the Y-axis (Up) in your basis transformation.\n");
	} else {
		printf("  [PASS] Ray direction looks plausible.\n");
	}

	return 0;
}
