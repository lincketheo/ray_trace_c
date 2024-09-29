
#include "obj_collection.h"
#include "ray.h"
#include "vec3.h"

typedef struct {
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

  // Derived
  int pxh;
  v3 center;
  v3 pixel00_loc;
  v3 pixel_delta_u;
  v3 pixel_delta_v;
  v3 u, v, w;
  v3 defocus_disk_u;
  v3 defocus_disk_v;
} world;

// Generates default values
world world_create_default();

void world_render_ppm(world *w, obj_collection *objs);
