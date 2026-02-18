#ifndef RAY_H
#define RAY_H

#include "vector3.h"

namespace raze
{
    class Ray {
    public:
        Ray() {}
        Ray(Vector3f origin, Vector3f direction) : _Origin(origin), _Direction(direction) {}

        const Vector3f& origin() const { return _Origin; }
        const Vector3f& direction() const { return _Direction; }

        Vector3f at(float t) const { return _Origin + _Direction * t; }

    private:
        Vector3f _Origin;
        Vector3f _Direction;
    };
    
}

#endif // RAY_H