#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "aabb.h"
#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

namespace raze
{
    class HittableList : public Hittable {
    public:
        HittableList() {};
        HittableList(shared_ptr<Hittable> object) { add(object); };

        void clear() { objects.clear(); }
        void add(shared_ptr<Hittable> object) 
        { 
            objects.push_back(object); 
            _BBox = AABB(_BBox, object->boundingBox());
        }

        bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const override
        {
            HitInfo temp_record;
            bool hit_anything = false;
            float closest_so_far = ray_t.max;

            for (const shared_ptr<Hittable>& object : objects)
            {
                if (object->hit(ray, Interval<float>(ray_t.min, closest_so_far), temp_record))
                {
                    hit_anything = true;
                    closest_so_far = temp_record.t;
                    record = temp_record;
                }
            }

            return hit_anything;
        }

        AABB boundingBox() const
        {
            return _BBox;
        }

    public:
        AABB _BBox;
        std::vector<shared_ptr<Hittable>> objects;
    };
}

#endif // HITTABLE_LIST_H