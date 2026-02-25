#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"

#include "vector3.h"

namespace raze
{
    enum class ImageFormat { PPM, JPG };

    struct Config
    {
        int image_width = 100;
        int samples_per_pixel = 1;
        int max_depth = 1;
        float aspect_ratio = 1.f;
        float vertival_fov = 60.f;
        float defocus_angle = 0.f;
        float focus_distance = 10.f;
        bool shade_with_normals = false;
        Vector3f look_from;
        Vector3f look_at;
        std::string image_name;
        ImageFormat format;
        int channel_number = 1;
        bool multithreading = false;
    };

    inline Config loadConfigFromJSON(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cout << "Could not load configuration from file: " << path << std::endl;
            return {};
        }
        
        nlohmann::json data = nlohmann::json::parse(file);
        auto configuration = data["config"];
        file.close();

        Config c;
        c.image_width               = configuration["image_width"].get<int>();
        c.samples_per_pixel         = configuration["samples_per_pixel"].get<int>();
        c.max_depth                 = configuration["max_depth"].get<int>();
        c.aspect_ratio              = configuration["aspect_ratio"].get<float>();
        c.vertival_fov              = configuration["vertival_fov"].get<float>();
        c.defocus_angle             = configuration["defocus_angle"].get<float>();
        c.focus_distance            = configuration["focus_distance"].get<float>();
        c.shade_with_normals        = configuration["shade_with_normals"].get<bool>();

        auto look_from = configuration["look_from"];
        auto look_at   = configuration["look_at"];

        float x = look_from["x"].get<float>();
        float y = look_from["y"].get<float>();
        float z = look_from["z"].get<float>();
        c.look_from = Vector3f(x, y, z);

        x = look_at["x"].get<float>();
        y = look_at["y"].get<float>();
        z = look_at["z"].get<float>();
        c.look_at = Vector3f(x, y, z);

        c.image_name = configuration["image_name"].get<std::string>();
        std::string image_format = configuration["image_format"].get<std::string>();
        
        if (image_format == "ppm")
        {
            c.format = ImageFormat::PPM;
        }
        else if(image_format == "jpg")
        {
            c.format = ImageFormat::JPG;
        }

        c.channel_number = configuration["channel_number"].get<int>();
        c.multithreading = configuration["multithreading"].get<bool>();

        return c;
    }
}

#endif // CONFIG_H