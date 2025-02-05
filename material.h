//[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
public:
    virtual ~material() = default;

    virtual color emitted(double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction, r_in.time());
        //attenuation = albedo / scatter_prob;
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<texture> tex;
};

class metal : public material {
public:
    metal(const color& albedo, double fuzz) : 
        tex(make_shared<solid_color>(albedo)), 
        spec_map(fuzz < 1 ? make_shared<solid_color>(color(fuzz)) : make_shared<solid_color>(color(1.0))) {}
    metal(shared_ptr<texture> tex, double fuzz) : 
        tex(tex), 
        spec_map(fuzz < 1 ? make_shared<solid_color>(color(fuzz)) : make_shared<solid_color>(color(1.0))) { }
    metal(const color& albedo, shared_ptr<texture> map) : 
        tex(make_shared<solid_color>(albedo)), 
        spec_map(map) {} 
    metal(shared_ptr<texture> tex, shared_ptr<texture> map) : 
        tex(tex),
        spec_map(map) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        attenuation = tex->value(rec.u, rec.v, rec.p);
        double spec = 1.0 - spec_map->value(rec.u, rec.v, rec.p).x(); //higher value -> more shiny

        vec3 reflected = reflect(r_in.direction(), rec.normal);
        //reflected = unit_vector(reflected) + (fuzz * spec * random_unit_vector());
        reflected = unit_vector(reflected) + (spec * random_unit_vector());
        //reflected = unit_vector(reflected) + ((1.0 - attenuation.x()) * random_unit_vector());
        scattered = ray(rec.p, reflected, r_in.time());
        //attenuation = albedo;
        //attenuation = tex->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    shared_ptr<texture> tex;
    shared_ptr<texture> spec_map;
};

class dielectric : public material {
public: //replace refraction_index with map
    dielectric(double refraction_index) : 
        refract_map(make_shared<solid_color>(color(refraction_index))), 
        tex(make_shared<solid_color>(color(1.0))){}
    dielectric(double refraction_index, const color& tint) :
        refract_map(make_shared<solid_color>(color(refraction_index))),
        tex(make_shared<solid_color>(tint)) {}
    dielectric(double refraction_index, shared_ptr<texture> tex) :
        refract_map(make_shared<solid_color>(color(refraction_index))),
        tex(tex) {}
    dielectric(const color& tint, shared_ptr<texture> map) :
        refract_map(map),
        tex(make_shared<solid_color>(color(tint))){ }
    dielectric(shared_ptr<texture> tex, shared_ptr<texture> map) :
        refract_map(map),
        tex(tex){ }

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        attenuation = tex->value(rec.u, rec.v, rec.p);
        vec3 refract_mapping = refract_map->value(rec.u, rec.v, rec.p);
        double refraction_index = refract_mapping.x() == 0.0 ? 0.0001: 2 * refract_mapping.x();

        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
        //double ri = rec.front_face ? (1.0 / test) : test;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

private:
    shared_ptr<texture> refract_map; //take in grayscale textures
    shared_ptr<texture> tex;

    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> tex) : tex(tex) {}

    diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

    color emitted(double u, double v, const point3& p) const override{
        return tex->value(u, v, p);
    }

private:
    shared_ptr<texture> tex;
};

class isotropic : public material {
public:
    isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
    isotropic(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        scattered = ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<texture> tex;
};

#endif