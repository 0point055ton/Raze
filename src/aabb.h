#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vector3.h"
#include "interval.h"

namespace raze
{
    class AABB {
    public:
        AABB() {}

        AABB(const Interval<float>& interval) : x(interval), y(interval), z(interval) {}

        AABB(const Interval<float>& x, const Interval<float>& y, const Interval<float>& z) :
            x(x), y(y), z(z) { padToMinimum(); }

        AABB(const Vector3f& a, const Vector3f& b)
        {
            x = a.x <= b.x ? Interval<float>(a.x, b.x) : Interval<float>(b.x, a.x);
            y = a.y <= b.y ? Interval<float>(a.y, b.y) : Interval<float>(b.y, a.y);
            z = a.z <= b.z ? Interval<float>(a.z, b.z) : Interval<float>(b.z, a.z);

            padToMinimum();
        }

        AABB(const AABB& box_a, const AABB& box_b)
        {
            x = Interval<float>(box_a.x, box_b.x);
            y = Interval<float>(box_a.y, box_b.y);
            z = Interval<float>(box_a.z, box_b.z);

            padToMinimum();
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
                const float adinv = 1.f / ray.direction().at(i);  // TODO: maybe division with close to zero?

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

        void padToMinimum()
        {
            if (x.size() < PADDING) x = x.expand(PADDING);
            if (y.size() < PADDING) y = y.expand(PADDING);
            if (z.size() < PADDING) z = z.expand(PADDING);
        }

    private:
        Interval<float> x, y, z;
    };
}

#endif // AABB_H