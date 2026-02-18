#ifndef CONFIG_H
#define CONFIG_H

#include "vector3.h"

namespace raze
{
    struct Config
    {
        int image_width = 100;
        int samples_per_pixel = 1;
        int max_depth = 1;
        float aspect_ratio = 1.f;
        float vertival_fov = 60.f;
        float defocus_angle = 0.f;
        float focus_distance = 10.f;
        Vector3f look_from;
        Vector3f look_at;
    };
}

#endif // CONFIG_H