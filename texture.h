//[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "rtw_stb_image.h"
#include "perlin.h"

class texture {
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(const color& albedo) : albedo(albedo) {}

    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    color value(double u, double v, const point3& p) const override {
        return albedo;
    }

private:
    color albedo;
};

class checker_texture : public texture {
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {
    }

    checker_texture(double scale, const color& c1, const color& c2)
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {
    }

    color value(double u, double v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
    image_texture(const char* filename) : image(filename), intensity(1.0) {}

    image_texture(const char* filename, double val) : image(filename), intensity(val) {}

    color value(double u, double v, const point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        //return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        return color_scale * intensity * color(pixel[0], pixel[1], pixel[2]);
    }

private:
    rtw_image image;
    double intensity;
};

class noise_texture : public texture {
public:
    noise_texture(double scale) : scale(scale), intensity(1.0), col(color(1.0)) {}
    noise_texture(double scale, color col) : scale(scale), intensity(1.0), col(col) {}
    noise_texture(double scale, double intensity) : scale(scale), intensity(intensity), col(color(1.0)) {}
    noise_texture() : scale(1.0), intensity(1.0), col(color(1.0)){}

    color value(double u, double v, const point3& p) const override {
        // no turbulence
        //return color(1,1,1) * 0.5 * (1.0 + noise.noise(scale * p));
        // straight turbulence
        //return color(1, 1, 1) * noise.turb(p, 7);
        // marble texture
        return intensity * (0.5 * col) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
    }

private:
    perlin noise;
    double scale;
    double intensity;
    color col;
};

#endif