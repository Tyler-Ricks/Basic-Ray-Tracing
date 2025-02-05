//[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "rtweekend.h"

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
        : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat)
    {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(static_center - rvec, static_center + rvec);
    }

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius,
        shared_ptr<material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat)
    {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(center.at(0) - rvec, center.at(0) + rvec);
        aabb box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = aabb(box1, box2);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        //takes t value and the point at a hit
        rec.t = root;
        rec.p = r.at(rec.t);
        //radius is the length of the normal vector P - C
        vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    ray center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

inline shared_ptr<hittable_list> packing(const point3& center, double R, const double r) {
    //generate a hollow glass ball, then fill it uniformly with smaller spheres

    auto spheres = make_shared<hittable_list>();

    if (R < r || R <= 0.1) { exit; }
    
    spheres->add(make_shared<sphere>(center, R, make_shared<dielectric>(1.60)));
    R -= 0.1; //decrement R to create the inner sphere
    spheres->add(make_shared<sphere>(center, R, make_shared<dielectric>(1.00 / 1.60)));

    //for each "spot" in a cube, add spheres that can be generated within the glass ball
    for (double x = center.x() - R; x <= center.x() + R; x += (r * 2)) {
        for (double y = center.y() - R; y <= center.y() + R; y += (r * 2)) {
            for (double z = center.z() - R; z <= center.z() + R; z += (r * 2)) {
                if (R >= (point3(x, y, z) - center).length() + r) {
                    shared_ptr<material> material_gumball;
                    double randmat = random_double();
                    if (randmat < 0.25) { material_gumball = make_shared<lambertian>(vec3::random());}

                    else if (randmat < 0.5) { material_gumball = make_shared<metal>(vec3::random(0.0, 1.0), random_double(0.0, 0.5)); }

                    else if (randmat < 0.625) {
                        auto noise = make_shared<noise_texture>(random_double(1.0, 10.0));
                        material_gumball = make_shared<metal>(noise, random_double(0.0, 0.5));
                    }
                    else if (randmat < 0.75) {material_gumball = make_shared<lambertian>(make_shared<noise_texture>(random_double(1.0, 4.0)));}

                    else if (randmat < 0.95) { material_gumball = make_shared<dielectric>(random_double(1.25, 1.75)); }

                    else if (randmat < 0.98) { material_gumball = make_shared<diffuse_light>(vec3::random() * random_double(1.0, 15.0));}

                    else {
                        auto tex = make_shared<image_texture>("rainbow1.jpg", random_double(1.0, 7.0));
                        material_gumball = make_shared<diffuse_light>(tex);
                    }

                    spheres->add(make_shared<sphere>(point3(x, y, z), r, material_gumball));
                }
            }
        }
    }
    
    return(spheres);
}

#endif