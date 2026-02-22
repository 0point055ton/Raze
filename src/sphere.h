#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

namespace raze
{
    class Sphere : public Hittable {
    public:
        Sphere(const Vector3f& center, float radius, shared_ptr<Material> material) :
            _Center(center), _Radius(std::fmaxf(0.f, radius)), _Material(material)
        {
            Vector3f radius_vector(radius);
            _BBox = AABB(center - radius_vector, center + radius_vector);
        }

        bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const override
        {
            Vector3f oc = _Center - ray.origin();
            float a = ray.direction().lengthSquared();
            float h = dot(ray.direction(), oc);
            float c = oc.lengthSquared() - _Radius * _Radius;
            float discriminant = h * h - a * c;
            
            if (discriminant < 0.f)
                return false;

            float sqrt_disc = std::sqrtf(discriminant);

            float root = (h - sqrt_disc) / a;
            if (!ray_t.surrounds(root))
            {
                root = (h + sqrt_disc) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            record.material = _Material;
            record.t = root;
            record.point = ray.at(root);
            Vector3f outward_normal = (record.point - _Center) / _Radius;
            record.setFaceNormal(ray, outward_normal);

            return true;
        }

        AABB boundingBox() const override
        {
            return _BBox;
        }

    private:
        Vector3f _Center;
        AABB     _BBox;
        float    _Radius;
        std::shared_ptr<Material> _Material;
    };
}

#endif // SPHERE_H