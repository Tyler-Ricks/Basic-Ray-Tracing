#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    //basic
    //initialize world
    hittable_list world;

    //hittable objects
    auto material_ground = make_shared<metal>(color(0.2, 0.8, 0.2), 0.0);
    auto material_center = make_shared<metal>(color(0.1, 0.2, 0.5), 0.0);
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_in_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_out_bubble = make_shared<dielectric>(1.50);

    auto trap_in = make_shared<dielectric>(1.00 / 1.50);
    auto trap_out = make_shared<dielectric>(1.50);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.2, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.4, material_in_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_out_bubble));


    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.4, material_in_bubble));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_out_bubble));

    //initialize camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 90;
    cam.lookfrom = point3(0.0, 0.0, -2.0);
    cam.lookat = point3(0, 0, -1);
    //cam.lookat = point3(-1.0, 0.0, -1.0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.5;

    cam.focus_dist = (cam.lookfrom - point3(0.0, 0.0, 0.0)).length();

    cam.render(world);
}