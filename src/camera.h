#ifndef CAMERA_H
#define CAMERA_H

#include "vector3.h"
#include "utility.h"

namespace raze
{
    struct Camera
    {
        Vector3f look_from;
        Vector3f look_at;
        Vector3f vup;
        Vector3f u, v, w;
        Vector3f defocus_disc_u;
        Vector3f defocus_disc_v;
        Vector3f pixel_delta_u;
        Vector3f pixel_delta_v;
        Vector3f pixel00_loc;

        Camera(const Config& config) 
        {
            vup = Vector3(0.f, 1.f, 0.f);
            
            look_from = config.look_from;
            look_at = config.look_at;

            int image_width = config.image_width;
            int image_height = int(image_width / config.aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            
            float theta = degToRad(config.vertival_fov);
            float h = std::tanf(theta / 2.f);

            float viewport_height = 2.f * h * config.focus_distance;
            float viewport_width = viewport_height * (float(image_width) / image_height);

            w = unitVector(look_from - look_at);
            u = unitVector(cross(vup, w));
            v = cross(w, u);

            Vector3f viewport_u = viewport_width * u;
            Vector3f viewport_v = viewport_height * -v;

            pixel_delta_u = viewport_u / float(image_width);
            pixel_delta_v = viewport_v / float(image_height);

            Vector3f vieport_upper_left = look_from 
                - (config.focus_distance * w) 
                - viewport_u / 2.f - viewport_v / 2.f;

            pixel00_loc = vieport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

            float defocus_radius = config.focus_distance * std::tanf(degToRad(config.defocus_angle / 2.f));
            defocus_disc_u = u * defocus_radius;
            defocus_disc_v = v * defocus_radius;
        }
    };
}

#endif // CAMERA_H