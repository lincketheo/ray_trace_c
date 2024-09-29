#include <float.h>
#include <stdio.h>

#include "exception.h"
#include "hit_result.h"
#include "obj_collection.h"
#include "ppm.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"
#include "world.h"


int main() {
  obj_collection objs = objc_create();

  for(int a = -11; a < 11; ++a) {
    for(int b = -11; b < 11; ++b) {
      double choose_mat = randd();
      v3 center = v3c(a + 0.9 * randd(), 0.2, b + 0.9 * randd());

      if(v3n2s(v3s(center, v3c(5, 0.2, 0))) > 0.9) {
        if(choose_mat < 0.8) {
          v3 albedo = v3hp(v3randd(), v3randd());
          material mat = (material) {
            .t = LAMB,
            .l = (lambertian) {
              .albedo = albedo
            }
          };
          objc_add_sphere(&objs, (sphere){
            .center = center,
            .radius = 0.2,
            .mat = mat 
          });
        } else if(choose_mat < 0.95) {
          v3 albedo = v3randdr(0.5, 1);
          double fuzz = randdr(0, 0.5);
          material mat = (material) {
            .t = METAL,
            .m = (metal) {
              .albedo = albedo,
              .fuzz = fuzz 
            }
          };
          objc_add_sphere(&objs, (sphere){
            .center = center,
            .radius = 0.2,
            .mat = mat 
          });
        } else {
          material mat = mat_dielecc(1.5);
          objc_add_sphere(&objs, (sphere){
            .center = center,
            .radius = 0.2,
            .mat = mat 
          });
        }
      }
    }
  }

  material material1 = mat_dielecc(1.5);
  objc_add_sphere(&objs, sc(0, 1, 0, 1.0, material1));

  material material2 = mat_lambc(0.4, 0.2, 0.1);
  objc_add_sphere(&objs, sc(-4, 1, 0, 1.0, material2));

  material material3 = mat_metalc(0.7, 0.6, 0.5, 0.0);
  objc_add_sphere(&objs, sc(4, 1, 0, 1.0, material3));


  world w = world_create_default();
  w.ar = 16.0 / 9.0;
  w.pxw = 1200;
  w.samples_per_pixel = 500;
  w.max_depth = 50;

  w.vfov = 20;
  w.lookfrom = v3c(13, 2, 3);
  w.lookat = v3c(0, 0, 0);
  w.vup = v3c(0, 1, 0);

  w.defocus_angle = 0.6;
  w.focus_dist = 10.0;

  world_render_ppm(&w, &objs);

  return 0;
}



/**
int main() {
  world w = world_create_default();

  material ground = mat_lambc(0.8, 0.8, 0);
  material center = mat_lambc(0.1, 0.2, 0.5);
  material left = mat_dielecc(1.5);
  material bubble = mat_dielecc(1.0 / 1.5);
  material right = mat_metalc(0.8, 0.6, 0.2, 1.0);

  obj_collection objs = objc_create();
  objc_add_sphere(&objs, sc(0.0, -100.5, -1.0, 100.0, ground));
  objc_add_sphere(&objs, sc(0.0, 0, -1.2, 0.5, center));
  objc_add_sphere(&objs, sc(-1.0, 0, -1.0, 0.5, left));
  objc_add_sphere(&objs, sc(-1.0, 0, -1.0, 0.4, bubble));
  objc_add_sphere(&objs, sc(1.0, 0, -1.0, 0.5, right));

  w.ar = 16.0 / 9.0;
  w.pxw = 400;
  w.samples_per_pixel = 100;
  w.max_depth = 50;

  w.vfov = 20;
  w.lookfrom = v3c(-2, 2, 1);
  w.lookat = v3c(0, 0, -1);
  w.vup = v3c(0, 1, 0);

  w.defocus_angle = 10.0;
  w.focus_dist = 3.4;

  world_render_ppm(&w, &objs);

  return 0;
}
*/

/**
int main() {
  double R = cos(M_PI/4);

  world w = world_create_default();

  material left = mat_lambc(1, 0, 0);
  material right = mat_lambc(0, 0, 1);

  obj_collection objs = objc_create();
  objc_add_sphere(&objs, sc(-R, 0, -1, R, left));
  objc_add_sphere(&objs, sc(R, 0, -1, R, right));

  w.ar = 16.0 / 9.0;
  w.pxw = 400;
  w.samples_per_pixel = 100;
  w.max_depth = 50;

  w.vfov = 90;

  world_render_ppm(&w, &objs);

  return 0;
}
*/
