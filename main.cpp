#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>


double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r) {
    //return color(0, 0, 0);
    //return r.direction()[0] > r.origin()[0] ? color(0, 0, 0) : color(0.5, 0, 0);

    auto ccenter = point3(0, 0, -1);
    auto t = hit_sphere(ccenter, 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - ccenter);
        return 0.5 * color(N[0] + 1, N[1] + 1, N[2] + 1);
    }

    auto dir = unit_vector(r.direction());
    auto a = 0.5 * (dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main() {

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    if (image_height < 1) {
        image_height = 1;
    }

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (
            static_cast<double>(image_width) / image_height);

    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);
    auto delta_u = viewport_u / image_width;
    auto delta_v = viewport_v / image_height;

    auto viewport_center = camera_center + vec3(0, 0, -focal_length);
    auto viewport_upper_left = viewport_center - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + delta_u/2 + delta_v/2;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * delta_u) + (j * delta_v);
            auto ray_dir = pixel_center - camera_center;
            ray r(camera_center, ray_dir);
            auto pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}

