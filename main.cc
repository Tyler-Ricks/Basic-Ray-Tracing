#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "bvh.h"
#include "texture.h"
#include "constant_medium.h"

hittable_list world;

//case 1
void spooky_render() {
    //spooky render
    //hittable_list world;

    //initialize camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0, 2.0, -0.8);
    cam.lookat = point3(0, 1, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.2;
    cam.focus_dist = (point3(0, 1, 2) - cam.lookfrom).length();
    cam.bg1 = color(0.70, 0.80, 1.00);

    auto material_ground = make_shared<metal>(color(0.9), 0.0);
    world.add(make_shared<sphere>(point3(0.0, -1000, 0.0), 1000.0, material_ground));
    //weird thing
    world.add(make_shared<sphere>(point3(0.0, 1002, 0.0), 1000.0, material_ground)); //bottom at y = 2

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
        //world.add(make_shared<sphere>(ray1.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble1_center - ray_origin).length()), bubble_radius, material_glow));
        world.add(make_shared<sphere>(ray2.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble2_center - ray_origin).length()), bubble_radius, material_glow));
        world.add(make_shared<sphere>(ray3.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble3_center - ray_origin).length()), bubble_radius, material_glow));
        world.add(make_shared<sphere>(ray4.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble4_center - ray_origin).length()), bubble_radius, material_glow));
        //world.add(make_shared<sphere>(ray5.at((2 * bubble_radius) * (i + 1) + 0.01 + (bubble5_center - ray_origin).length()), bubble_radius, material_glow));
        //world.add(make_shared<sphere>(point3(0.0, 1.0, 2*i + 2), 1.0, material_inner));
    }

    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world);
}
//case 2
void scatter_spheres() {
    auto ground_noise = make_shared<noise_texture>(2.0);
    auto ground_material = make_shared<metal>(ground_noise, 0.1);
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<metal>(color(0.8), 0.1)));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));


    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            //auto choose_mat = random_double();
            point3 center(0.9 * (a + random_double()), 0.2, 0.9 * (b + random_double()));
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                double randmat1 = random_double();
                double randmat = random_double();

                if (randmat < 0.26) { //subsurface
                    sphere_material = make_shared<dielectric>(1.5);
                    auto bound = make_shared<sphere>(center, 0.19, make_shared<lambertian>(color(1.0)));
                    world.add(make_shared<constant_medium>(bound, random_double(3.0, 7.0), vec3::random(0.5, 1.0)));
                }

                else if (randmat < 0.4) { sphere_material = make_shared<metal>(vec3::random(0.0, 1.0), random_double(0.0, 0.5)); }

                else if (randmat < 0.55) { 
                    auto noise = make_shared<noise_texture>(random_double(10.0, 50.0));
                    sphere_material = make_shared<metal>(noise, random_double(0.0, 0.2));
                }

                else if (randmat < 0.77) { sphere_material = make_shared<dielectric>(1.5, make_shared<noise_texture>(random_double(10.0, 50.0))); }

                else if (randmat < 0.89) { sphere_material = make_shared<dielectric>(random_double(1.25, 1.75)); }

                else if (randmat < 0.99) { sphere_material = make_shared<diffuse_light>(vec3::random() * vec3::random() * random_double(1.0, 15.0)); }

                else {
                    auto tex = make_shared<image_texture>("rainbow1.jpg", random_double(1.0, 7.0));
                    sphere_material = make_shared<diffuse_light>(tex);
                }
                //auto noise = make_shared<noise_texture>(random_double(30.0, 50.0));
                //sphere_material = make_shared<metal>(noise, random_double(0.0, 0.3));
                //sphere_material = make_shared<lambertian>(noise);

                world.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 1000;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(-0.75, 2, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    //cam.bg1 = color(1.0);
    //cam.bg2 - color(1.0);

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 3
void full_render() {
    //hittable_list world;

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

    cam.bg1 = color(0.70, 0.80, 1.00);

    //full render

    //ground 
    //auto material_ground = make_shared<metal>(color(0.9), 0.0);
    //world.add(make_shared<sphere>(point3(0.0, -1000, 0.0), 1000.0, material_ground));
    auto checker = make_shared<checker_texture>(0.32, color(0.263, 0.416, 0.557), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<metal>(checker, 0.0)));

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
                    else{
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

    //auto material_big_metal = make_shared<metal>(color(0.5, 0.5, 0.5), 0.0);
    //world.add(make_shared<sphere>(metal_center, metal_radius, material_big_metal));
    //auto material_big_metal = make_shared<checker_texture>(0.1, color(0.25), color(0.5));
    auto material_big_metal = make_shared<image_texture>("dumpy.jpg");
    world.add(make_shared<sphere>(metal_center, metal_radius, make_shared<metal>(material_big_metal, 0.0)));

    double glossy_radius = 1.0;
    point3 glossy_center = point3(1.8, glossy_radius, -3.2);
    auto material_glossy_outer = make_shared<dielectric>(1.6);
    world.add(make_shared<sphere>(glossy_center, glossy_radius, material_glossy_outer));

    //auto material_glossy_inner = make_shared<lambertian>(color(0.75, 0.75, 0.75));
    auto material_glossy_texture = make_shared<image_texture>("earthmap.jpg");
    auto material_glossy_inner = make_shared<lambertian>(material_glossy_texture);
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

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 4
void test() {
    //hittable_list world;

    //initialize camera

    auto per1 = make_shared<noise_texture>(0.5);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<metal>(color(0.5), per1)));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(per1)));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(1.0))));
    //world.add(make_shared<sphere>(point3(0, 1, 1), 1, make_shared<lambertian>(color(1.0, 0.0, 0.0))));
    //world.add(make_shared<sphere>(point3(-2, 1, 1), 1, make_shared<dielectric>(1.5, color(1.0, 0.84, 1.0))));
    //world.add(make_shared<sphere>(point3(-2, 1, 1), 0.9, make_shared<dielectric>(1 / 1.5, color(1.0, 0.84, 1.0))));
    //world.add(make_shared<sphere>(point3(2, 1, 1), 1, make_shared<metal>(color(0.8), 0.1)));

    auto per = make_shared<noise_texture>(20.0);
    auto perlinglass = make_shared<dielectric>(color(0.75, 0.635, 1.0), per);
    world.add(make_shared<sphere>(point3(0, 1, 4.5), 1, perlinglass));
    //world.add(make_shared<sphere>(point3(0, 1, 0), 1, make_shared<lambertian>(color(1.0, 0.0, 0.0))));
    //world.add(make_shared<sphere>(point3(0, 1, 4.5), 1, make_shared<lambertian>(color(1.0, 0.0, 0.0))));  

    //world.add(make_shared<sphere>(point3(-3, 1, 0), 1, make_shared<metal>(per, per)));

    //world.add(make_shared<sphere>(point3(0, 0.2, 1.3), 0.2, make_shared<lambertian>(color(0.0, 1.0, 0.0))));
    //world.add(make_shared<sphere>(point3(-0.4, 0.2, 1.3), 0.2, make_shared<lambertian>(color(1.0, 0.0, 0.0))));
    //world.add(make_shared<sphere>(point3(0.4, 0.2, 1.3), 0.2, make_shared<lambertian>(color(0.0, 0.0, 1.0))));

    //world.add(make_shared<sphere>(point3(0, 1, 0), 0.1, make_shared<diffuse_light>(make_shared<image_texture>("rainbow1.jpg", 20.0))));

    world.add(make_shared<quad>(point3(-1, 3.5, 4), vec3(2, 0, 0), vec3(0, 0, 2), make_shared<diffuse_light>(color(10.0))));
    

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 5000;
    cam.max_depth = 50;
    //cam.bg1 = color(1.0);
    //cam.bg2 = cam.bg1;

    cam.vfov = 70;
    //cam.lookfrom = point3(13, 3, 4);
    cam.lookfrom = point3(0, 2, 1.5);
    cam.lookat = point3(0, 1, 3.5);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();

    cam.render(world);
}
//case 5
void checkered_spheres() {
    //hittable_list world;

    //auto checker1 = make_shared<checker_texture>(0.25, color(0.0, 0.0, 0.0), color(0.56f, 0.85f, 0.676f));
    auto image = make_shared<image_texture>("rainbow2");
    auto image_surface = make_shared<metal>(image, 0.1);
    auto checker2 = make_shared<checker_texture>(0.32, color(0.9), color(0.1));


    //world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker1)));
    //world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<metal>(checker1, 0.05)));
    world.add(make_shared<sphere>(point3(0, -5, 0), 5, make_shared<metal>(checker2, 0.0)));
    world.add(make_shared<sphere>(point3(0, 5, 0), 5, image_surface));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 13);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.bg1 = color(0.70, 0.80, 1.00);

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 6
void image_to_sphere() {
    auto earth_texture = make_shared<image_texture>("dumpy.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.bg1 = color(0.70, 0.80, 1.00);

    cam.render(hittable_list(globe));
}
//case 7
void perlin_spheres() {
    //hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    //auto pertext2 = make_shared<noise_texture>(1);
    //auto metal_perlin = make_shared<metal>(pertext2, 0.1);
    //world.add(make_shared<sphere>(point3(0, 2, 0), 2, metal_perlin));
    
    auto metaball = make_shared<metal>(color(0.8), 0.0);
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, metaball));

    //world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
    //auto tex = make_shared<image_texture>("rainbow2.jpg");
    //auto surface = make_shared<metal>(tex, 0.25);
    //world.add(make_shared<sphere>(point3(0, 2, 0), 2, surface));


    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(17, 5, -10);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.bg1 = color(0.70, 0.80, 1.00);

    cam.render(world);
}
//case 8
void quads() {
    //hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    //auto back = make_shared<metal>(color(0.8), 0.05);
    auto back_tex = make_shared<noise_texture>(4);
    auto back = make_shared<metal>(back_tex, 0.05);
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    //auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto upper_tex = make_shared<image_texture>("dumpy.jpg");
    auto upper = make_shared<lambertian>(upper_tex);
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));
    auto front_glass = make_shared<dielectric>(1.5);

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2.75, -2.75, 1), vec3(5.5, 0, 0), vec3(0, 5.5, 0), back));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    //world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));
    world.add(make_shared<quad>(point3(-2.75, -2.75, 6), vec3(5.5, 0, 0), vec3(0, 5.5, 0), front_glass));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.bg1 = color(0.70, 0.80, 1.00);

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 9
void buncha_glass() {
    //hittable_list world;

    auto mirror = make_shared<metal>(color(0.8), 0.05);
    auto glass = make_shared<dielectric>(1.5);

    world.add(make_shared<quad>(point3(-2.75, -2.75, 1), vec3(5.5, 0, 0), vec3(0, 5.5, 0), mirror));

    for (double i = -3.0; i <= 3.0; i+=0.25) {
        world.add(make_shared<quad>(point3(-3, i, 1), vec3(6, 0, 0), vec3(0, 0, 4), glass));
    }

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    //cam.lookfrom = point3(0, -5, 1);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 10
void simple_light() {
    //hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(1.0,1.0,1.0))));

    auto tex = make_shared<image_texture>("layer1.png", 4.0);
    auto surface = make_shared<diffuse_light>(tex);
    auto difflight = make_shared<diffuse_light>(color(4, 0, 0));
    world.add(make_shared<quad>(point3(3, 1, -2), vec3(4, 0, 0), vec3(0, 4, 0), surface));
    //world.add(make_shared<sphere>(point3(3, 1, -2), 1.0, surface));
    //world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.bg1 = color(0, 0, 0);

    cam.vfov = 20;
    cam.lookfrom = point3(26, 3, 6);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}
//case 11
void cornell_box() {
    //hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto metal_floor = make_shared<metal>(color(0.8), 0.0);
    auto tex = make_shared<image_texture>("rainbow.jpg", 8.0);
    auto surface = make_shared<diffuse_light>(tex);

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    //world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(make_shared<quad>(point3(150, 554, 150), vec3(255, 0, 0), vec3(0, 0, 255), light));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    //world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), metal_floor)); //floor
    world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));
    //world.add(make_shared<sphere>(point3(335, 50, 188), 50.0, surface));
    world.add(make_shared<sphere>(point3(335, 50, 188), 50.0, metal_floor));

    auto glass = make_shared<dielectric>(1.5);
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), glass);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600; //600
    cam.samples_per_pixel = 1000; //500
    cam.max_depth = 50;
    cam.bg1 = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world);
}
//case 12
void cornell_smoke() {
    //hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.bg1 = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}
//case 13
void spheres_in_the_dark() {
    
    //ground
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<metal>(color(0.1), 0.0)));

    //rainbow light
    auto tex = make_shared<image_texture>("gradient1.jpg", 5.0);
    auto surface = make_shared<diffuse_light>(tex);
    shared_ptr<hittable> rainsphere = make_shared<sphere>(point3(0.0), 1.0, surface);
    rainsphere = make_shared<rotate_y>(rainsphere, -90);
    rainsphere = make_shared<translate>(rainsphere, vec3(3, 1, -2));
    world.add(rainsphere);

    //sphere packing
    //shared_ptr<hittable> pack = packing(point3(0, 2, 0), 2, 0.2);
    //world.add(make_shared<bvh_node>(pack));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(make_shared<noise_texture>(2.0))));

    //perlin texture light sphere
    auto pertext = make_shared<noise_texture>(2, 0.05);
    auto perlight = make_shared<diffuse_light>(pertext);
    //world.add(make_shared<sphere>(point3(-4, 3, 2.66), 3, perlight));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.bg1 = color(0, 0, 0);
    //cam.bg1 = color(1.0);

    cam.vfov = 20;
    cam.lookfrom = point3(19, 3, 7);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();

    cam.render(world);
}
//case 14
void rgb_squares() {
    hittable_list world;

    //ground
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(1.0))));

    auto light1 = make_shared<diffuse_light>(make_shared<image_texture>("rainbow1.jpg", 20.0));
    auto light2 = make_shared<diffuse_light>(color(10.0));
    //world.add(make_shared<sphere>(point3(0, 1.0, 0), 0.1, light1));

    world.add(make_shared<quad>(point3(-1, 1.001, -1), vec3(2, 0, 0), vec3(0, 0, 2), make_shared<lambertian>(color(1.0))));
    world.add(make_shared<quad>(point3(-1, 1, -1), vec3(2, 0, 0), vec3(0, 0, 2), light2));

    auto glass_pane = make_shared<dielectric>(1.5, make_shared<image_texture>("rainbow1.jpg"));
    //auto glass_pane = make_shared<dielectric>(1.5, make_shared<noise_texture>(20.0));
    //auto glass_pane = make_shared<dielectric>(1.5, color(0.1));
    
    //world.add(make_shared<quad>(point3(-2, 0.5, -2), vec3(4, 0, 0), vec3(0, 0, 4), glass_pane));
    world.add(make_shared<quad>(point3(-1, 0.99, -1), vec3(2, 0, 0), vec3(0, 0, 2), glass_pane));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.bg1 = color(0.0);

    cam.vfov = 20;
    //cam.lookfrom = point3(13, 4, 3.5);
    cam.lookfrom = point3(3.5, 4, 13);
    cam.lookat = point3(0, 1, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();

    cam.render(world);
}
//case 15
void cornell_with_sphere_packing() {
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto metal_floor = make_shared<metal>(color(0.8), 0.0);

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    //world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(make_shared<quad>(point3(150, 554, 150), vec3(255, 0, 0), vec3(0, 0, 255), light));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white)); //floor
    //world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), metal_floor)); //floor
    world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white)); //cieling
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white)); //back panel
    //world.add(make_shared<sphere>(point3(335, 50, 188), 50.0, surface));
    //world.add(make_shared<sphere>(point3(335, 50, 188), 50.0, metal_floor));

    //auto glass = make_shared<dielectric>(1.5);
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), metal_floor);
    //auto pack = make_shared<sphere>(point3(265, 115, 295), 115, white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    //shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), glass);
    //box2 = make_shared<rotate_y>(box2, -18);
    //box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    //auto pack = make_shared<sphere>(point3(130, 85, 65), 85, white);
    //pack = make_shared<rotate_y>(pack, -18);
    //pack = make_shared<translate>(pack, vec3(130, 0, 65));
    shared_ptr<hittable> pack = packing(point3(0, 0, 0), 85, 10);
    pack = make_shared<rotate_y>(pack, -18);
    pack = make_shared<translate>(pack, vec3(160, 85, 65));
    world.add(make_shared<bvh_node>(pack));
    //world.add(pack);
    //world.add(box2);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600; //600
    cam.samples_per_pixel = 1000; //500
    cam.max_depth = 50;
    cam.bg1 = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world);
}
//case 16
void testing2() {
    auto ground_noise = make_shared<noise_texture>(2.0);
    auto ground_material = make_shared<metal>(ground_noise, 0.1);
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<metal>(color(0.8), 0.1)));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));


    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            //auto choose_mat = random_double();
            point3 center(0.9 * (a + random_double()), 0.2, 0.9 * (b + random_double()));
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                double randmat = random_double();
                if (randmat < 0.25) { sphere_material = make_shared<lambertian>(vec3::random() * vec3::random()); }

                else if (randmat < 0.4) { sphere_material = make_shared<metal>(vec3::random(0.0, 1.0), random_double(0.0, 0.5)); }

                else if (randmat < 0.55) {
                    auto noise = make_shared<noise_texture>(random_double(30.0, 50.0));
                    sphere_material = make_shared<metal>(noise, random_double(0.0, 0.2));
                }
                else if (randmat < 0.7) { sphere_material = make_shared<lambertian>(make_shared<noise_texture>(random_double(30.0, 50.0))); }

                else if (randmat < 0.84) { sphere_material = make_shared<dielectric>(random_double(1.25, 1.75)); }

                else if (randmat < 0.98) { sphere_material = make_shared<diffuse_light>(vec3::random() * vec3::random() * random_double(1.0, 10.0)); }

                else {
                    auto tex = make_shared<image_texture>("rainbow1.jpg", random_double(1.0, 7.0));
                    sphere_material = make_shared<diffuse_light>(tex);
                }
                //auto noise = make_shared<noise_texture>(random_double(30.0, 50.0));
                //sphere_material = make_shared<metal>(noise, random_double(0.0, 0.3));
                //sphere_material = make_shared<lambertian>(noise);

                world.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }
    auto boundary = make_shared<sphere>(point3(0), 1000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, 0.005, color(1.0)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.bg1 = color(0.0);
    cam.bg2 = color(0.0, 0.0, 0.01);
    //cam.bg1 = color(1.0, 0.35, 0.25);
    //cam.bg2 = color(0, 0, 1);

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 17
void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2), 15),
        vec3(-100, 270, 395)
    )
    );

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth = max_depth;
    cam.bg1 = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}
//case 18
void aligned_spheres() {

    auto ground = make_shared<lambertian>(make_shared<noise_texture>(1.0));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(0.263, 0.416, 1.0))));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 999.8, ground));
    
    auto mirror = make_shared<metal>(color(0.8), 0.15);
    auto glass = make_shared<dielectric>(1.5, color(0.0, 1.0, 0.75));

    double root_three = 1.73205;
    for (int i = -4; i <= 3; i++) { //z
        for (int j = -3; j <= 3; j++) {
            if ((i >= -1 && i <= 1) && (j == 0)) { continue; }
            world.add(make_shared<sphere>(point3(j * (2 * root_three), 1, 2 * i), 1, mirror));
        }

        for (int k = -3; k <= 3; k++) {
            if ((i == 0 || i == 1) && (k == 0)) { continue; }
            world.add(make_shared<sphere>(point3(k * (2 * root_three) + root_three, 1, 2 * i + 1), 1, mirror));
            //world.add(make_shared<sphere>(point3(k * (2 * root_three) + root_three, 1, 2 * i + 1), 1, glass));
        }
    }

    point3 lightpos1 = point3(4.5, 4, 6);
    //point3 lightpos1 = point3(17.82, 10, 0.32);
    point3 lightpos2 = point3(3.6, 15, 12.9);
    point3 lightpos3 = point3(7.5, 25, 8.12);

    auto light1 = make_shared<diffuse_light>(color(15.0));
    auto light2 = make_shared<diffuse_light>(20 * color(1, 0.91, 0.17));
    auto light3 = make_shared<diffuse_light>(65 * color(0.8, 0.4, 0.2));

    world.add(make_shared<sphere>(lightpos1, 1, light1));
    world.add(make_shared<sphere>(lightpos2, 1.3, light2));
    world.add(make_shared<sphere>(lightpos3, 1, light3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 1000;
    cam.max_depth = 50;

    cam.vfov = 75;
    cam.lookfrom = point3(2.62, 4, 4.55);
    ///cam.lookfrom = point3(0.866, 10, 1.414);
    //cam.lookat = point3(0.866, 1, 1.414);
    cam.lookat = point3(1.44, 1, 2.63);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();

    //cam.bg1 = color(1.0);
    //cam.bg2 = color(0.0, 0.0, 0.05);

    //cam.bg1 = color(0.75);
    //cam.bg2 = color(0.25);
    //cam.bg1 = color(1.0, 0.35, 0.25);
    //cam.bg2 = color(0, 0, 1);

    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 19
void perlin_glass_sphere() {
    hittable_list world;

    //ground
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(1.0))));

    auto light1 = make_shared<diffuse_light>(make_shared<image_texture>("rainbow1.jpg", 40.0));
    auto light2 = make_shared<diffuse_light>(color(10.0));

    world.add(make_shared<sphere>(point3(0, 1, 0), 0.1, light1));
    //off-screen
    world.add(make_shared<quad>(point3(-0.5, 4, -0.5), vec3(1, 0, 0), vec3(0, 0, 1), light2));

    //noise texture on glass
    auto glass_test = make_shared<dielectric>(1.5, make_shared<noise_texture>(20.0));
    world.add(make_shared<sphere>(point3(0, 1, 0), 1, glass_test));

    auto fog = make_shared<sphere>(point3(0, 0, 0), 50, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(fog, .05, color(1, 1, 1)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 5000;
    cam.max_depth = 50;
    cam.bg1 = color(0.0);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 3, 4);
    cam.lookat = point3(0, 1, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = (cam.lookfrom - cam.lookat).length();

    cam.render(world);
}
//case 20
void pillars() {

    //auto mat_pillar = make_shared<lambertian>(color(0.56, 0.85, 0.676));
    auto mat_pillar = make_shared<dielectric>(1.5);
    auto pillar_color = make_shared<lambertian>(color(1.0));

    shared_ptr<hittable> box_origin = box(point3(1, 0, 1), point3(-1, -100, -1), mat_pillar);
    shared_ptr<hittable> box_inner = box(point3(0.9, 0, 0.9), point3(-0.9, -100, -0.9), pillar_color);
    
    hittable_list boxes;
    for (int i = -5; i <= 5; i++) {
        for (int j = -2; j <= 5; j++) {
            auto box1 = make_shared<translate>(box_origin, vec3(4 * i, 0, 4 * j));
            auto box2 = make_shared<translate>(box_inner, vec3(4 * i, 0, 4 * j));
            boxes.add(box1);
            //boxes.add(make_shared<constant_medium>(box2, 1.0, color(1,0,0)));
            boxes.add(box2);
        }
    }

    world.add(make_shared<bvh_node>(boxes));
    /*for (int j = -5; j <= 5; j++) {
        auto box1 = make_shared<translate>(box_origin, vec3(0, 0, 4 * j));
        auto box2 = make_shared<translate>(box_inner, vec3(0, 0, 4 * j));
        world.add(box1);
        world.add(make_shared<constant_medium>(box2, 1.0, color(1, 0, 0)));
    }*/
    //world.add(box1);
    //world.add(make_shared<box>(point3(-1, -100, -1), point3(1, 0, 1), white));

    world.add(make_shared<sphere>(point3(0, 1.1, 0), 1.0, make_shared<diffuse_light>(make_shared<noise_texture>(2.0, 4.0))));

    world.add(make_shared<quad>(point3(-2, 20, -1), vec3(4, 0, 0), vec3(0, 0, 4), make_shared<diffuse_light>(color(15.0))));

    //auto fog = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    //world.add(make_shared<constant_medium>(fog, .0002, color(1, 1, 1)));

    //world.add(make_shared<quad>(point3(-1.5, 1.5, -1.5), vec3(3, 0, 0), vec3(0, 0, 3), make_shared<diffuse_light>(color(1.0))));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 500;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0,4.0, -10.0);
    cam.lookat = point3(0, 1.0, 0);
    cam.vup = vec3(0, 1, 0);
    //cam.bg1 = color(0.70, 0.80, 1.00);
    //cam.bg2 = cam.bg1;

    //world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 21
void subsurface() {
    hittable_list world;

    //gorund
    auto ground_mat = make_shared<lambertian>(color(1.0));
    //auto ground_mat = make_shared<lambertian>(color(1.0));

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));

    auto light1 = make_shared<diffuse_light>(color(2.0));
    //world.add(make_shared<sphere>(point3(1.5, 2.75, 0.75), 1.0, light1));

    //auto light2 = make_shared<diffuse_light>(10 * color(1, 0.91, 0.17));
    auto light2 = make_shared<diffuse_light>(10 * color(1.0));
    //world.add(make_shared<sphere>(point3(-2, 1.5, 3.0), 1, light2));
    world.add(make_shared<sphere>(point3(0, 4.1, 0), 1, light2));

    //world.add(make_shared<sphere>(point3(2, 3, 1.5), 1, light2));

    auto outer_mat = make_shared<dielectric>(1.5);
    auto volume_mat = make_shared<dielectric>(1.5);
    //shared_ptr<hittable> boundary = make_shared<sphere>(point3(0, 2, 0), 2.0, volume_mat);
    //auto inner = make_shared<constant_medium>(boundary, 1.5, color(1.0, 0.0, 0.0));
    shared_ptr<hittable> box_outer = box(point3(-1.5, 0, -1.5), point3(1.5, 3, 1.5), outer_mat);
    shared_ptr<hittable> box_inner = box(point3(-1.49, 0.01, -1.49), point3(1.49, 2.99, 1.49), volume_mat);
    box_outer = make_shared<rotate_y>(box_outer, -25);
    box_inner = make_shared<rotate_y>(box_inner, -25);
    world.add(box_outer);
    world.add(make_shared<constant_medium>(box_inner, 2.0, color(1.0, 0.0, 0.0)));
    
    //world.add(boundary);
    //world.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 2, outer_mat));
    //world.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 1.8, make_shared<dielectric>(1.0 / 1.5)));
    //world.add(inner);
    //world.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 2, make_shared<lambertian>(color(1.0, 0.0, 0.0))));

    //world.add(make_shared<sphere>(point3(-0.2, 0, )))


    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 5000;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0, 2.0, -5.0);
    cam.lookat = point3(0, 2.0, 0);
    cam.vup = vec3(0, 2, 0);
    //cam.bg1 = color(0.70, 0.80, 1.00);
    //cam.bg2 = cam.bg1;

    //world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}
//case 22
void weird_cubes() {
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(1.0))));

    hittable_list cube1;
    auto layer1 = box(point3(-1, 0, -1), point3(1, 1.25, 1), make_shared<lambertian>(color(0.9)));
    auto layer2 = box(point3(-1, 1.2501, -1), point3(1, 1.45, 1), make_shared<diffuse_light>(13 * color(0.0, 1.0, 1.0)));
    auto layer3 = box(point3(-1, 1.4501, -1), point3(1, 1.95, 1), make_shared<dielectric>(1.5, color(1.0, 0.84, 1.0)));
    auto layer4 = box(point3(-1, 1.9501, -1), point3(1, 1.97, 1), make_shared<metal>(color(1, 0.876, 0.708), 0.0));
    auto layer5 = box(point3(-1, 1.975, -1), point3(1, 2, 1), make_shared<metal>(color(1, 0.876, 0.708), 0.0));
    cube1.add(layer1);
    cube1.add(layer2);
    cube1.add(layer3);
    cube1.add(layer4);
    cube1.add(layer5);

    world.add(make_shared<rotate_y>(make_shared<bvh_node>(cube1), -25));

    point3 lightpos1 = point3(3, 4, 6);
    //point3 lightpos1 = point3(17.82, 10, 0.32);
    point3 lightpos2 = point3(-4, 10, -4);
    point3 lightpos3 = point3(2, 7, -12);

    auto light1 = make_shared<diffuse_light>(color(15.0));
    auto light2 = make_shared<diffuse_light>(20 * color(1, 0.91, 0.7));
    auto light3 = make_shared<diffuse_light>(65 * color(0.8, 0.4, 0.2));

    //world.add(make_shared<sphere>(lightpos1, 1, light1));
    //world.add(make_shared<sphere>(lightpos2, 1.3, light2));
    //world.add(make_shared<sphere>(lightpos3, 1, light3));
    world.add(make_shared<quad>(point3(-1, 4, -1), vec3(2, 0, 0), vec3(0, 0, 2), light1));

    shared_ptr<hittable> light = make_shared<quad>(point3(-1, 1, 0), vec3(2, 0, 0), vec3(0, 2, 0), light1);
    light = make_shared<translate>(light, vec3(0, 0, -6));
    light = make_shared<rotate_y>(light, -45);
    world.add(light);


    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 5000;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0, 2.5, -3.0);
    cam.lookat = point3(0, 1.0, 0);
    cam.vup = vec3(0, 1, 0);
    //cam.bg2 = color(0.70, 0.80, 1.00);
    //cam.bg1 = color(1.0);

    //world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
}

int main() {
    /*hittable_list world;

    //initialize camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;

    cam.vfov = 90;
    //cam.lookfrom = point3(0, 1, -1);
    cam.lookfrom = point3(0, 1.5, -0.8);
    cam.lookat = point3(0, 1, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.2;
    cam.focus_dist = (point3(0, 1, 2) - cam.lookfrom).length();

    //auto material_ground = make_shared<metal>(color(0.9), 0.0);
    auto material_ground = make_shared<lambertian>(color(1.0));
    world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, material_ground));


    cam.render(world);*/

    switch (4) {
        case 1:  spooky_render(); break;
        case 2:  scatter_spheres(); break;
        case 3:  full_render(); break;
        case 4:  test(); break;
        case 5:  checkered_spheres(); break;
        case 6:  image_to_sphere(); break;
        case 7:  perlin_spheres(); break;
        case 8:  quads(); break;
        case 9:  buncha_glass(); break;
        case 10: simple_light(); break;
        case 11: cornell_box(); break;
        case 12: cornell_smoke(); break;
        case 13: spheres_in_the_dark(); break;
        case 14: rgb_squares(); break;
        case 15: cornell_with_sphere_packing(); break;
        case 16: testing2(); break;
        case 17: final_scene(400, 250, 4); break;
        case 18: aligned_spheres(); break;
        case 19: perlin_glass_sphere(); break;
        case 20: pillars(); break;
        case 21: subsurface(); break;
        case 22: weird_cubes(); break;
    }
    return(0);

}