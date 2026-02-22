#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vector3.h"
#include "interval.h"

namespace raze
{
    struct AABB
    {
        Interval<float> x, y, z;

        AABB() {}

        AABB(const Interval<float>& interval) : x(interval), y(interval), z(interval) {}

        AABB(const Interval<float>& x, const Interval<float>& y, const Interval<float>& z) :
            x(x), y(y), z(z) {}

        AABB(const Vector3f& a, const Vector3f& b)
        {
            x = a.x <= b.x ? Interval<float>(a.x, b.x) : Interval<float>(b.x, a.x);
            y = a.y <= b.y ? Interval<float>(a.y, b.y) : Interval<float>(b.y, a.y);
            z = a.z <= b.z ? Interval<float>(a.z, b.z) : Interval<float>(b.z, a.z);
        }

        AABB(const AABB& box_a, const AABB& box_b)
        {
            x = Interval<float>(box_a.x, box_b.x);
            y = Interval<float>(box_a.y, box_b.y);
            z = Interval<float>(box_a.z, box_b.z);
        }

        const Interval<float>& axisInterval(int axis) const
        {
            if (axis == 1) return y;
            if (axis == 2) return z;
            return x;
        }

        bool hit(const Ray& ray, Interval<float> ray_t) const
        {
            for (int i = 0; i < 3; ++i)
            {
                const Interval<float>& axis = axisInterval(i);
                const float adinv = 1.f / ray.direction().at(i);

                float t0 = (axis.min - ray.origin().at(i)) * adinv;
                float t1 = (axis.max - ray.origin().at(i)) * adinv;

                if (t0 < t1)
                {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                }
                else
                {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min)
                    return false;
            }
            return true;
        }

        int longestAxis() const
        {
            if (x.size() > y.size())
            {
                return x.size() > z.size() ? 0 : 2;
            }
            else
            {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        // static const AABB empty, universe;
    };

    // const AABB AABB::empty = AABB(Interval<float>(INF, -INF), Interval<float>(INF, -INF), Interval<float>(INF, -INF));
    // const AABB AABB::universe = AABB(Interval<float>(-INF, INF), Interval<float>(-INF, INF), Interval<float>(-INF, INF));
}

#endif // AABB_H