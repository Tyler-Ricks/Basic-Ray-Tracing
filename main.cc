#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    //initialize camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0, 1.5, -0.8);
    cam.lookat = point3(0, 1, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.2;
    cam.focus_dist = (point3(0, 1, 2) - cam.lookfrom).length();

    //basic
    /*//initialize world
    hittable_list world;

    //hittable objects
    auto material_ground = make_shared<lambertian>(color(0.2, 0.8, 0.2));
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
    //cam.focus_dist = 3.4;
    //cam.focus_dist = (cam.lookfrom - cam.lookat).length();
    cam.focus_dist = (cam.lookfrom - point3(0.0, 0.0, 0.0)).length();*/

    /*hittable_list world;

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, -5);
    cam.lookat = point3(0, 1.5, -0.5);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;



    //full render

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
            point3 center = point3(x + 0.6*random_double(), random_radius, z + 0.6*random_double());

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
     }*/

    hittable_list world;
    //auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_ground = make_shared<metal>(color(0.9), 0.0);
    world.add(make_shared<sphere>(point3(0.0, -1000, 0.0), 1000.0, material_ground));

    //snow
    /*
    //bottom snowball
    auto snow = make_shared<lambertian>(color(1.0, 1.0, 1.0));

    world.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 2.0, snow));
    world.add(make_shared<sphere>(point3(0.0, 5.2, 0.0), 1.5, snow));
    world.add(make_shared<sphere>(point3(0.0, 7.4, 0.0), 1, snow));

    //bunch of falling snow

    for (int i = 0; i < 300; i++) {
        double random_x = random_double(-10,10);
        double random_z = random_double(-10, 10);
        double random_y = random_double(0, 20);

        world.add(make_shared<sphere>(point3(random_x, random_y, random_z), 0.1, snow));
    }

    //initialize camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth = 500;

    cam.vfov = 60;
    cam.lookfrom = point3(0, 4, -10);
    cam.lookat = point3(0, 5, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.2;
    cam.focus_dist = (point3(0,5,0) - cam.lookfrom).length();

    cam.render(world);*/


    // spooky render
    
    //front bubbles, left to right
    auto material_outer = make_shared<dielectric>(1.5);
    auto material_inner = make_shared<dielectric>(1 / 1.6);
    //point3 ray_origin = cam.lookfrom - vec3(0.0, 1.5, 0.0);
    double bubble_radius = 0.7;
    //point3 ray_origin = point3(cam.lookfrom.x(), 1.0, cam.lookfrom.z());
    point3 ray_origin = point3(0, 1, 0);
    //cam.lookfrom = point3(0, 7, 0);
    

    /*point3 bubble1_center = point3(-4.0, 1.0, 0.0);
    world.add(make_shared<sphere>(bubble1_center, bubble_radius, material_outer));
    world.add(make_shared<sphere>(bubble1_center, bubble_radius - 0.1, material_inner));
    ray ray1 = ray(ray_origin, unit_vector(bubble1_center - ray_origin));*/

    point3 bubble2_center = point3(-1.5, 1.0, 1.0);
    world.add(make_shared<sphere>(bubble2_center, bubble_radius, material_outer));
    world.add(make_shared<sphere>(bubble2_center, bubble_radius - 0.15, material_inner));
    ray ray2 = ray(ray_origin, unit_vector(bubble2_center - ray_origin));

    point3 bubble3_center = point3(0.0, 1.0, 1.5); 
    world.add(make_shared<sphere>(bubble3_center, bubble_radius, material_outer));
    world.add(make_shared<sphere>(bubble3_center, bubble_radius - 0.15, material_inner));
    ray ray3 = ray(ray_origin, unit_vector(bubble3_center - ray_origin));

    point3 bubble4_center = point3(1.5, 1.0, 1.0);
    world.add(make_shared<sphere>(bubble4_center, bubble_radius, material_outer));
    world.add(make_shared<sphere>(bubble4_center, bubble_radius - 0.15, material_inner));
    ray ray4 = ray(ray_origin, unit_vector(bubble4_center - ray_origin));

    /*point3 bubble5_center = point3(4.0, 1.0, 0.0);
    world.add(make_shared<sphere>(bubble5_center, bubble_radius, material_outer));
    world.add(make_shared<sphere>(bubble5_center, bubble_radius - 0.1, material_inner));
    ray ray5 = ray(ray_origin, unit_vector(bubble5_center - ray_origin));*/
    
    auto material_glow = make_shared<lambertian>(color(0.56, 0.85, 0.676));
    //auto material_glow = make_shared<lambertian>(color(0.75, 0.75, 0.75));
    //int cam_to_mid = bubble3_center.z() - ray_origin.z();

    for (int i = 0; i < 20; i++) {
        //world.add(make_shared<sphere>(ray1.at((2*bubble_radius)*(i+1) + cam_to_mid), bubble_radius, material_glow));
        //world.add(make_shared<sphere>(ray2.at(2 * i + 0.7 + 5), 0.7, material_glow));
        world.add(make_shared<sphere>(ray2.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble2_center - ray_origin).length()), bubble_radius, material_glow));
        world.add(make_shared<sphere>(ray3.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble3_center - ray_origin).length()), bubble_radius, material_glow));
        world.add(make_shared<sphere>(ray4.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble4_center - ray_origin).length()), bubble_radius, material_glow));
        //world.add(make_shared<sphere>(ray5.at((2 * bubble_radius) * (i + 1) + cam_to_mid), bubble_radius, material_glow));
        //world.add(make_shared<sphere>(point3(0.0, 1.0, 2*i + 2), 1.0, material_inner));
    }


    // sphere packing
    /*point3 bubble_center = vec3(0.0, 1.5, 1.0);
    double big_radius = 1.4;

    auto material_in_bubble = make_shared<dielectric>(1.00 / 1.60);
    auto material_out_bubble = make_shared<dielectric>(1.60);

    world.add(make_shared<sphere>(bubble_center, big_radius, material_in_bubble));
    world.add(make_shared<sphere>(bubble_center, 1.5, material_out_bubble)); //to (0,0,0.5)

    double gumball_radius = 0.5;

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
    }*/

    //book render
    /*hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    //cam.render(world);*/


    //weird thing
    world.add(make_shared<sphere>(point3(0.0, 1002, 0.0), 1000.0, material_ground)); //bottom at y = 2

    //auto material_random_balls = make_shared<lambertian>(color(1.0, 0.0, 0.0));
    //world.add(make_shared<sphere>(point3(0.5, 0.3, 1.0), 0.2, material_random_balls));

    /*for (int i = 0; i < 50; i++) {
        double random_radius = random_double();
        double random_x = random_double(-10, 10);
        double random_y = random_double(random_radius, 2 - random_radius);
        double random_z = random_double(-10, 20);
        double random_mat = random_double();

        if (random_mat < 0.50) {
            auto material_random = make_shared<lambertian>(vec3::random());
            world.add(make_shared<sphere>(point3(random_x, random_y, random_z), random_radius, material_random));
        }
        if (random_mat < 0.80) {
            auto material_random = make_shared<metal>(vec3::random(0,0.5), random_double(0,0.3));
            world.add(make_shared<sphere>(point3(random_x, random_y, random_z), random_radius, material_random));
        }
        if (random_mat < 90) {
            auto material_random = make_shared<dielectric>(1.5);
            world.add(make_shared<sphere>(point3(random_x, random_y, random_z), random_radius, material_random));
        }
        else {
            auto material_random_outer = make_shared<dielectric>(1.5);
            world.add(make_shared<sphere>(point3(random_x, random_y, random_z), random_radius, material_random_outer));
            auto material_random_inner = make_shared<dielectric>(1/1.5);
            world.add(make_shared<sphere>(point3(random_x, random_y, random_z), random_radius - 0.05, material_random_inner));
        }
    }*/

    /*for (int x = -10; x < 10; x += 2) {
        for (int z = -10; z < 10; z += 2) {
            double random_radius = random_double(0.1, 0.5);
            point3 center = point3(x + random_double(0, random_radius), random_double(random_radius, 2 - random_radius), z + random_double(0, random_radius));
            double random_mat = random_double();
            /*if (random_mat < 0.50) {
                auto material_random = make_shared<lambertian>(vec3::random());
                world.add(make_shared<sphere>(center, random_radius, material_random));
            }
            if (random_mat < 0.80) {
                auto material_random = make_shared<metal>(vec3::random(0, 0.5), random_double(0, 0.3));
                world.add(make_shared<sphere>(center, random_radius, material_random));
            }
            if (random_mat < 0.40) {
                auto material_random = make_shared<dielectric>(1.5);
                world.add(make_shared<sphere>(center, random_radius, material_random));
            }
            else if (random_mat < 0.80) {
                auto material_random_outer = make_shared<dielectric>(1.5);
                world.add(make_shared<sphere>(center, random_radius, material_random_outer));
                auto material_random_inner = make_shared<dielectric>(1 / 1.5);
                world.add(make_shared<sphere>(center, random_radius - 0.09, material_random_inner));
            }
        }
    }*/
    /*auto material_random_outer = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,-8), 0.5, material_random_outer));
    auto material_random_inner = make_shared<dielectric>(1 / 1.5);
    world.add(make_shared<sphere>(point3(0, 1, -8), 0.5 - 0.1, material_random_inner));*/

    cam.render(world);
}