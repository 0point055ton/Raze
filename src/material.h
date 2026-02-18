#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "random.h"

namespace raze
{
    class Material {
    public:
        virtual ~Material() = default;
        
        virtual bool scatter(
            const Ray& ray_in, const HitInfo& record, Vector3f& attenuation, Ray& scattered
            ) const
        { 
            (void)ray_in;
            (void)record;
            (void)attenuation;
            (void)scattered;
            return false; 
        }
    };

    /////////////////////////////////////

    class Lambertian : public Material {
    public:
        Lambertian(const Vector3f& albedo) : _Albedo(albedo) {}
        
        bool scatter(const Ray& ray_in, const HitInfo& record, Vector3f& attenuation, Ray& scattered) 
            const override
        { 
            (void)ray_in;

            Vector3f scatter_direction = record.normal + Random::getUnitVector3f();
            if (scatter_direction.nearZero())
                scatter_direction = record.normal;

            scattered = Ray(record.point, scatter_direction);
            attenuation = _Albedo;
            return true; 
        }

    private:
        Vector3f _Albedo;
    };

    /////////////////////////////////////

    class Metal : public Material {
    public:
        Metal(const Vector3f& albedo, float fuzz) : _Albedo(albedo), _Fuzz(fuzz < 1.f ? fuzz : 1.f) {}
        
        bool scatter(const Ray& ray_in, const HitInfo& record, Vector3f& attenuation, Ray& scattered) 
            const override
        { 
            Vector3f reflected = reflect(ray_in.direction(), record.normal);
            reflected = unitVector(reflected) + (_Fuzz * Random::getUnitVector3f());
            scattered = Ray(record.point, reflected);
            attenuation = _Albedo;
            return (dot(scattered.direction(), record.normal) > 0.f); 
        }

    private:
        Vector3f _Albedo;
        float    _Fuzz;
    };

    /////////////////////////////////////

    class Dielectric : public Material {
    public:
        Dielectric(float refraction_index) : _RefractionIndex(refraction_index) {}
        
        bool scatter(const Ray& ray_in, const HitInfo& record, Vector3f& attenuation, Ray& scattered) 
            const override
        { 
            float ri = record.front_face ? (1.f / _RefractionIndex) : _RefractionIndex;
            Vector3f unit_direction = unitVector(ray_in.direction());
            float cos_theta = std::fminf(dot(-unit_direction, record.normal), 1.f);
            float sin_theta = std::sqrtf(1.f - cos_theta * cos_theta);

            bool cannot_refract = ri * sin_theta > 1.f;
            Vector3f direction;

            if (cannot_refract || reflectance(cos_theta, ri) > Random::getFloat())
                direction = reflect(unit_direction, record.normal);
            else
                direction = refract(unit_direction, record.normal, ri);

            attenuation = Vector3f(1.f);
            scattered = Ray(record.point, direction);
            return true; 
        }

    private:
        float _RefractionIndex;

        static float reflectance(float cosine, float refraction_index)
        {
            float r0 = (1.f - refraction_index) / (1.f + refraction_index);
            r0 = r0 * r0;
            return r0 + (1.f - r0) * std::powf((1.f - cosine), 5.f);
        }
    };
}

#endif // MATERIAL_H