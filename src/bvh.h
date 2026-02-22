#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "random.h"

namespace raze
{
    class BVH : public Hittable {
    public:
        BVH(HittableList list) : BVH(list.objects, 0, list.objects.size()) {}

        BVH(std::vector<std::shared_ptr<Hittable>> objects, size_t start, size_t end) 
        {
            // _BBox = AABB(Interval<float>(INF, -INF));

            // for (size_t i = start; i < end; ++i)
            // {
            //     _BBox = AABB(_BBox, objects[i]->boundingBox());
            // }
            // int axis = _BBox.longestAxis();
            int axis = Random::getInt(0, 2);

            bool (*comparator)(const std::shared_ptr<Hittable>&, const std::shared_ptr<Hittable>&);

            switch (axis)
            {
            case 0: comparator = boxCompareX; break;
            case 1: comparator = boxCompareY; break;
            case 2: comparator = boxCompareZ; break;
            default: comparator = boxCompareZ; break;
            }

            size_t object_span = end - start;

            if (object_span == 1)
            {
                _Left  = objects[start];
                _Right = objects[start];
            }
            else if (object_span == 2)
            {
                _Left  = objects[start];
                _Right = objects[start + 1];
            }
            else
            {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);

                size_t mid = start + object_span / 2;
                _Left = std::make_shared<BVH>(objects, start, mid);
                _Right = std::make_shared<BVH>(objects, mid, end);
            }

            _BBox = AABB(_Left->boundingBox(), _Right->boundingBox());
        }

        bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const override
        {
            if (!_BBox.hit(ray, ray_t))
                return false;

            bool hit_left = _Left->hit(ray, ray_t, record);
            bool hit_right = _Right->hit(ray, Interval<float>(ray_t.min, hit_left ? record.t : ray_t.max), record);
            
            return hit_left || hit_right;
        }

        AABB boundingBox() const override
        {
            return _BBox;
        }

    private:
        static bool boxCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis)
        {
            float a_min = a->boundingBox().axisInterval(axis).min;
            float b_min = b->boundingBox().axisInterval(axis).min;
            return a_min < b_min;
        }

        static bool boxCompareX(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
        {
            return boxCompare(a, b, 0);
        }

        static bool boxCompareY(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
        {
            return boxCompare(a, b, 1);
        }

        static bool boxCompareZ(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b)
        {
            return boxCompare(a, b, 2);
        }

    private:
        AABB _BBox;
        shared_ptr<Hittable> _Left;
        shared_ptr<Hittable> _Right;
    };
}

#endif // BVH_H