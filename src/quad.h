#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

namespace raze
{
    class Quad : public Hittable {
    public:
        Quad(const Vector3f& origin, const Vector3f& u, const Vector3f& v, std::shared_ptr<Material> material)
            : _Origin(origin), _U(u), _V(v), _Material(material) 
        { 
            Vector3f n = cross(u, v);
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
            // Only needed when doint UV mapping
            (void)record;

            Interval<float> unit_interval(0.f, 1.f);

            if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
                return false;

            return true;
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

#endif // QUAD_H