#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    hittable_list world;

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, -5);
    cam.lookat = point3(0, 1.5, -0.5);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    //full render

    //ground 
    auto material_ground = make_shared<metal>(color(0.9), 0.0);
    world.add(make_shared<sphere>(point3(0.0, -1000, 0.0), 1000.0, material_ground));

    // sphere packing
    double big_radius = 1.4;
    point3 bubble_center = vec3(0.0, 1.5, -1.0);

    auto material_in_bubble = make_shared<dielectric>(1.00 / 1.60);
    auto material_out_bubble = make_shared<dielectric>(1.60);

    world.add(make_shared<sphere>(bubble_center, big_radius, material_in_bubble));
    world.add(make_shared<sphere>(bubble_center, 1.5, material_out_bubble)); //to (0,0,0.5)

    double gumball_radius = 0.2;

    for (double x = bubble_center.x() - big_radius; x <= bubble_center.x() + big_radius; x += (gumball_radius * 2)) {
        for (double y = bubble_center.y() - big_radius; y <= bubble_center.y() + big_radius; y += (gumball_radius * 2)) {
            for (double z = bubble_center.z() - big_radius; z <= bubble_center.z() + big_radius; z += (gumball_radius * 2)) {
                if (big_radius >= (point3(x, y, z) - bubble_center).length() + gumball_radius) {
                    double randmat = random_double();
                    if (randmat <= 0.5) {
                        auto material_gumball = make_shared<lambertian>(vec3::random(0.0, 1.0));
                        world.add(make_shared<sphere>(point3(x, y, z), gumball_radius, material_gumball));
                    }
                    else if (randmat <= 0.9) {
                        auto material_gumball = make_shared<metal>(vec3::random(0.0, 1.0), random_double(0.0, 0.5));
                        world.add(make_shared<sphere>(point3(x, y, z), gumball_radius, material_gumball));
                    }
                    else {
                        auto material_gumball = make_shared<dielectric>(random_double(1.25, 1.75));
                        world.add(make_shared<sphere>(point3(x, y, z), gumball_radius, material_gumball));
                    }
                }
            }
        }
    }


    //other stuff
    double metal_radius = 2.0;
    point3 metal_center = point3(-2.5, metal_radius, 2.5);

    auto material_big_metal = make_shared<metal>(color(0.5, 0.5, 0.5), 0.0);
    world.add(make_shared<sphere>(metal_center, metal_radius, material_big_metal));


    double glossy_radius = 1.0;
    point3 glossy_center = point3(1.8, glossy_radius, -3.2);
    auto material_glossy_outer = make_shared<dielectric>(1.6);
    world.add(make_shared<sphere>(glossy_center, glossy_radius, material_glossy_outer));

    auto material_glossy_inner = make_shared<lambertian>(color(0.75, 0.75, 0.75));
    world.add(make_shared<sphere>(glossy_center, glossy_radius - 0.1, material_glossy_inner));


    //scattered balls
    for (int x = -7; x < 5; x++) {
        for (int z = -7; z < 8; z++) {
            double random_radius = random_double(0.1, 0.25);
            //double random_radius = 0.4;
            point3 center = point3(x + 0.6 * random_double(), random_radius, z + 0.6 * random_double());

            bool clip_metal = sphere_clip(center, random_radius, metal_center, metal_radius);
            bool clip_bubble = sphere_clip(center, random_radius, bubble_center, big_radius + 0.1);
            bool clip_glossy = sphere_clip(center, random_radius, glossy_center, glossy_radius);
            if (!(clip_metal || clip_bubble || clip_glossy)) {
                //add a check to ensure no collision with non scattered spheres
                double random_mat = random_double();
                if (random_mat <= 0.35) {
                    // lambertian 35% chance
                    auto material_scatter = make_shared<lambertian>(vec3::random(0.0, 1.0));
                    world.add(make_shared<sphere>(center, random_radius, material_scatter));
                }
                else if (random_mat <= 0.60) {
                    // metal 25% chance
                    double fuzz = random_double(0.0, 0.5);
                    auto material_scatter = make_shared<metal>(vec3::random(0.0, 1.0), fuzz);
                    world.add(make_shared<sphere>(center, random_radius, material_scatter));
                }
                else if (random_mat <= 0.75) {
                    // lambertian in a glass sphere 15% chance
                    //glass shell
                    auto material_scatter_glass = make_shared<dielectric>(random_double(1.45, 1.75));
                    world.add(make_shared<sphere>(center, random_radius, material_scatter_glass));
                    //lambertian inside
                    auto material_scatter = make_shared<lambertian>(vec3::random(0.0, 1.0));
                    world.add(make_shared<sphere>(center, random_radius - 0.01, material_scatter));
                }
                else if (random_mat <= 0.90) {
                    // metal in a glass sphere 15% chance
                    //glass shell
                    auto material_scatter_glass = make_shared<dielectric>(random_double(1.45, 1.75));
                    world.add(make_shared<sphere>(center, random_radius, material_scatter_glass));
                    //lambertian inside
                    double fuzz = random_double(0.0, 0.5);
                    auto material_scatter = make_shared<metal>(vec3::random(0.0, 1.0), fuzz);
                    world.add(make_shared<sphere>(center, random_radius - 0.01, material_scatter));
                }
                else {
                    // glass 10% chance
                    auto material_scatter = make_shared<dielectric>(random_double(1.45, 1.75));
                    world.add(make_shared<sphere>(center, random_radius, material_scatter));
                }
            }
        }
    }
    cam.render(world);
}