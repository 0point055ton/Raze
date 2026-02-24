#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

namespace raze
{
    class Triangle : public Hittable {
    public:
        Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, std::shared_ptr<Material> material)
            : _Origin(a), _U(b - a), _V(c - a), _Material(material) 
        { 
            Vector3f n = cross(_U, _V);
            _Normal = unitVector(n);
            _D = dot(_Normal, _Origin);
            _W = n / dot(n, n);

            setBoundingBox(); 
        }

        virtual void setBoundingBox() 
        {
            AABB diagonal_1 = AABB(_Origin, _Origin + _U + _V);
            AABB diagonal_2 = AABB(_Origin + _U, _Origin + _V);
            _BBox = AABB(diagonal_1, diagonal_2);
        }

        AABB boundingBox() const override { return _BBox; }

        bool hit(const Ray& ray, Interval<float> ray_t, HitInfo& record) const override
        {
            float denominator = dot(ray.direction(), _Normal);

            if (std::fabsf(denominator) < EPS)
                return false;

            float t = (_D - dot(ray.origin(), _Normal)) / denominator;

            if (!ray_t.contains(t))
                return false;

            Vector3f intersection = ray.at(t);
            Vector3f planar_hitpt_vector = intersection - _Origin;
            float alpha = dot(_W, cross(planar_hitpt_vector, _V)); 
            float beta  = dot(_W, cross(_U, planar_hitpt_vector)); 

            if (!isInterior(alpha, beta, record))
                return false;

            record.t = t;
            record.point = intersection;
            record.material = _Material;
            record.setFaceNormal(ray, _Normal);
            
            return true;
        }

        virtual bool isInterior(float alpha, float beta, HitInfo& record) const
        {
            (void)record;

            if (alpha > 0.f && beta > 0.f && alpha + beta < 1.f)
                return true;

            return false;
        }
        
    private:
        Vector3f _Origin;
        Vector3f _U, _V;
        Vector3f _W;
        Vector3f _Normal;
        std::shared_ptr<Material> _Material;
        AABB _BBox;
        float _D;
    };
}

#endif // TRIANGLE_H