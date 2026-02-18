#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "camera.h"
#include "random.h"
#include "material.h"

namespace raze
{
    Camera::Camera(Config config) : _Config(config), _LookFrom(0.f, 2.f, 3.f), _LookAt(0.f, 0.f, -1.f), _VUp(0.f, 1.f, 0.f) {}

    void Camera::render(const Hittable& world) 
    {
        initialize();

        auto start_time = std::chrono::system_clock::now();

        int prev_percentage = -1;
        for (int j = 0; j < _ImageHeight; ++j)
        {
            int percentage = (int)((j + 1) / float(_ImageHeight) * 100.f);
            if (percentage != prev_percentage)
            {
                std::cout << percentage << "% done\n";
                prev_percentage = percentage;
            }

            for (int i = 0; i < _ImageWidth; ++i)
            {
                Vector3f pixel_color;
                for (int sample = 0; sample < _Config.samples_per_pixel; ++sample)
                {
                    Ray ray = getRay(i, j);
                    pixel_color += rayColor(ray, _Config.max_depth, world);
                }

                Vector3f c = _PixelSampleScale * pixel_color;
                _FrameBuffer.emplace_back(c.x, c.y, c.z);
            }
        }

        auto end_time = std::chrono::system_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << "Finished rendering in " << elapsed_time.count() << " ms" << std::endl;
    }

    bool Camera::saveToPPM(const std::string& name) const
    {
        std::ofstream file(name);
        if (!file.is_open())
        {
            std::cout << "Could not open file: " << name << "\n";
            return false;
        }
        else
        {
            std::cout << "Saving to file: " << name << ".ppm\n";
        }

        file << "P3\n" << _ImageWidth << " " << _ImageHeight << "\n255\n";

        auto start_time = std::chrono::system_clock::now();

        for (size_t i = 0; i < _FrameBuffer.size(); ++i)
        {
            file << _FrameBuffer[i] << "\n";
        }

        auto end_time = std::chrono::system_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << "Saving took " << elapsed_time.count() << " ms" << std::endl;
        file.close();

        return true;
    }

    void Camera::initialize() 
    {
        _LookFrom = _Config.look_from;
        _LookAt = _Config.look_at;

        _ImageWidth = _Config.image_width;
        _ImageHeight = int(_ImageWidth / _Config.aspect_ratio);
        _ImageHeight = (_ImageHeight < 1) ? 1 : _ImageHeight;
        
        _Center = _LookFrom;
        // float focal_length = (_LookFrom - _LookAt).length();
        float theta = degToRad(_Config.vertival_fov);
        float h = std::tanf(theta / 2.f);

        float viewport_height = 2.f * h * _Config.focus_distance;
        float viewport_width = viewport_height * (float(_ImageWidth) / _ImageHeight);

        _W = unitVector(_LookFrom - _LookAt);
        _U = unitVector(cross(_VUp, _W));
        _V = cross(_W, _U);

        Vector3f viewport_u = viewport_width * _U;
        Vector3f viewport_v = viewport_height * -_V;

        _PixelDeltaU = viewport_u / float(_ImageWidth);
        _PixelDeltaV = viewport_v / float(_ImageHeight);

        Vector3f vieport_upper_left = _Center 
            - (_Config.focus_distance * _W) 
            - viewport_u / 2.f - viewport_v / 2.f;

        _Pixel00Loc = vieport_upper_left + 0.5f * (_PixelDeltaU + _PixelDeltaV);
        _PixelSampleScale = 1.f / _Config.samples_per_pixel;

        float defocus_radius = _Config.focus_distance * std::tanf(degToRad(_Config.defocus_angle / 2.f));
        _DefocusDiscU = _U * defocus_radius;
        _DefocusDiscV = _V * defocus_radius;

        _FrameBuffer.reserve(_ImageWidth * _ImageHeight);
    }

    Vector3f Camera::rayColor(const Ray& ray, int depth, const Hittable& world)
    {
        if (depth <= 0)
            return Vector3f(0.f);

        HitInfo record;
        if (world.hit(ray, Interval<float>(0.001f, INF), record))
        {
            Ray scattered;
            Vector3f attenuation;

            if (record.material->scatter(ray, record, attenuation, scattered))
                return attenuation * rayColor(scattered, depth - 1, world);

            return Vector3f(0.f);

            // Vector3f direction = record.normal + Random::getUnitVector3f();
            // return 0.5f * rayColor(Ray(record.point, direction), depth - 1, world);
        }
        
        Vector3f unit_direction = unitVector(ray.direction());
        float a = 0.5f * (unit_direction.y + 1.f);
        return (1.f - a) * Vector3f(1.f) + a * Vector3f(0.5f, 0.7f, 1.0f);
    }

    Vector3f Camera::sampleSquare()
    {
        return Vector3f(Random::getFloat() - 0.5f, Random::getFloat() - 0.5f, 0.f);
    }

    Ray Camera::getRay(int i, int j)
    {
        Vector3f offset = sampleSquare();
        Vector3f pixel_center = _Pixel00Loc + (float(i + offset.x) * _PixelDeltaU) + (float(j + offset.y) * _PixelDeltaV);

        Vector3f ray_origin = (_Config.defocus_angle <= 0.f) ? _Center : defocusDiscSample();
        Vector3f ray_direction = pixel_center - _Center;
        return Ray(ray_origin, ray_direction);
    }

    Vector3f Camera::defocusDiscSample()
    {
        Vector3f v = Random::getVector3fOnUnitDisk();
        return _Center + (v.x * _DefocusDiscU) + (v.y * _DefocusDiscV);
    }
}