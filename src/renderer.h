#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include "color.h"
#include "cam.h"
#include "hittable_list.h"

namespace raze
{
    class Renderer {
    public:
        Renderer(const Config& config);
        void render(const Hittable& world);

    private:
        void renderRegion(const Hittable& world, std::vector<Color> result);

    private:
        Config    _Config;
        Cam       _Camera;
        std::vector<Color>  _FrameBuffer;
    };
}

#endif // RENDERER_H