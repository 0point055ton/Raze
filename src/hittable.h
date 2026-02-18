#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>

#include "interval.h"
#include "ray.h"

namespace raze
{
    class Material;

    struct HitInfo
    {
        Vector3f point;
        Vector3f normal;
        float t;
        bool front_face;
        std::shared_ptr<Material> material;

        void setFaceNormal(const Ray& ray, const Vector3f& outward_normal)
        {
            // Assuming that outward_normal is of unit length
            front_face = dot(ray.direction(), outward_normal) < 0.f;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

    class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const = 0;
    };    
}

#endif // HITTABLE_H