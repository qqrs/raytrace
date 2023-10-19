#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            vec3 oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;
            auto discriminant = half_b*half_b - a*c;

            if (discriminant < 0)
                return false;

            auto sqrtd = sqrt(discriminant);
            double t = (-half_b - sqrtd) / a;
            if (t <= ray_tmin || t > ray_tmax) {
                t = (-half_b + sqrtd) / a;
                if (t <= ray_tmin || t > ray_tmax) {
                    return false;
                }
            }

            rec.t = t;
            rec.p = r.at(t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }

    private:
        point3 center;
        double radius;
};

#endif
