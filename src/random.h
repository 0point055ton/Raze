#ifndef RANDOM_H
#define RANDOM_H

#include <random>

#include "vector3.h"

namespace raze
{
    class Random {
    public:
        // Random() : _Generator(std::random_device{}()), _Distribution(0.f, 1.f) {}

        static float getFloat()
        {
            return _Distribution(_Generator);
        }

        static float getFloat(float min, float max)
        {
            return min + (max - min) * getFloat();
        }
        
        static Vector3f getVector3f()
        {
            return Vector3f(getFloat(), getFloat(), getFloat());
        }

        static Vector3f getVector3f(float min, float max)
        {
            return Vector3f(getFloat(min, max), getFloat(min, max), getFloat(min, max));
        }

        static Vector3f getUnitVector3f()
        {
            while (true)
            {
                Vector3f p = getVector3f(-1.f, 1.f);
                float len_sqrd = p.lengthSquared();
                if (len_sqrd > 1e-30 && len_sqrd <= 1.f)
                    return p / sqrtf(len_sqrd);
            }
        }

        static Vector3f getVector3fOnHemisphere(const Vector3f& normal)
        {
            Vector3f unit_vector = getUnitVector3f();
            if (dot(unit_vector, normal) > 0.f)
                return unit_vector;
            else
                return -unit_vector;
        }

        static Vector3f getVector3fOnUnitDisk()
        {
            while (true)
            {
                Vector3f v = Vector3f(getFloat(-1.f, 1.f), getFloat(-1.f, 1.f), 0.f);
                if (v.lengthSquared() < 1.f)
                    return v;
            }
        }

    private:
        static std::mt19937 _Generator;
        static std::uniform_real_distribution<float> _Distribution;
    };
}

#endif // RANDOM_H