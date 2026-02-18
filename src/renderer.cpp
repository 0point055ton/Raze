#include <iostream>
#include <fstream>

#include "material.h"
#include "renderer.h"
#include "random.h"

namespace raze
{
    Renderer::Renderer(const Config& config) : _Config(config), _Camera(config)
    {
        _ImageWidth = _Config.image_width;
        _ImageHeight = int(_ImageWidth / _Config.aspect_ratio);
        _ImageHeight = (_ImageHeight < 1) ? 1 : _ImageHeight;
        _PixelSampleScale = 1.f / _Config.samples_per_pixel;
    }

    void Renderer::render(const Hittable& world) 
    {
        auto start_time = std::chrono::system_clock::now();

        std::vector<Color> result1;
        std::vector<Color> result2;

        result1.reserve(370);
        result2.reserve(370);

        std::thread worker1(&Renderer::renderRegion, this, std::ref(world), std::ref(result1), 0, 360);
        std::thread worker2(&Renderer::renderRegion, this, std::ref(world), std::ref(result2), 361, 720);

        worker1.join();
        worker2.join();
        
        _FrameBuffer.reserve(result1.size() + result2.size());
        _FrameBuffer.insert(_FrameBuffer.end(), result1.begin(), result1.end());
        _FrameBuffer.insert(_FrameBuffer.end(), result2.begin(), result2.end());

        auto end_time = std::chrono::system_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

        std::cout << "Rendering finished in " << elapsed_time.count() << " seconds" << std::endl;
    }

    void Renderer::renderRegion(const Hittable& world, std::vector<Color>& result, size_t start, size_t end)
    {
        for (size_t j = start; j < end; ++j)
        {
            for (int i = 0; i < _ImageWidth; ++i)
            {
                Vector3f pixel_color;
                for (int sample = 0; sample < _Config.samples_per_pixel; ++sample)
                {
                    Ray ray = getRay(i, j);
                    pixel_color += rayColor(ray, _Config.max_depth, world);
                }

                Vector3f c = _PixelSampleScale * pixel_color;
                result.emplace_back(c.x, c.y, c.z);
            }
        }
    }

    Vector3f Renderer::rayColor(const Ray& ray, int depth, const Hittable& world) const
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
        }
        
        Vector3f unit_direction = unitVector(ray.direction());
        float a = 0.5f * (unit_direction.y + 1.f);
        return (1.f - a) * Vector3f(1.f) + a * Vector3f(0.5f, 0.7f, 1.0f);
    }

    Vector3f Renderer::sampleSquare() const
    {
        return Vector3f(Random::getFloat() - 0.5f, Random::getFloat() - 0.5f, 0.f);
    }

    Ray Renderer::getRay(int i, int j) const
    {
        Vector3f offset = sampleSquare();
        Vector3f pixel_center = _Camera.pixel00_loc + (float(i + offset.x) * _Camera.pixel_delta_u) + (float(j + offset.y) * _Camera.pixel_delta_v);

        Vector3f ray_origin = (_Config.defocus_angle <= 0.f) ? _Camera.look_from : defocusDiscSample();
        Vector3f ray_direction = pixel_center - _Camera.look_from;
        return Ray(ray_origin, ray_direction);
    }

    Vector3f Renderer::defocusDiscSample() const
    {
        Vector3f v = Random::getVector3fOnUnitDisk();
        return _Camera.look_from + (v.x * _Camera.defocus_disc_u) + (v.y * _Camera.defocus_disc_v);
    }

    bool Renderer::saveToPPM(const std::string& name) const
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
}