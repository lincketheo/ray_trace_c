
#include "obj_collection.h"
#include "ray.h"
#include "vec3.h"

typedef struct {
  // Parameters you can change
  double ar;
  int pxw;
  int samples_per_pixel;
  int max_depth;

  double vfov;
  v3 lookfrom;
  v3 lookat;
  v3 vup;

  double defocus_angle;
  double focus_dist;

  // Parameters you shouldn't change (Derived)
  int pxh;
  v3 center;
  v3 pixel00_loc;
  v3 pixel_delta_u;
  v3 pixel_delta_v;
  v3 u, v, w;
  v3 defocus_disk_u;
  v3 defocus_disk_v;
} camera;

// Generates default values - you can change values, then camera_render_ppm will 
// set everything correctly
camera camera_create_default();

// Render a camera and print to stdout in ppm format
void camera_render_ppm(camera* w, const obj_collection* objs);
