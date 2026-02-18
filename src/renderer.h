#ifndef RENDERER_H
#define RENDERER_H

#include <thread>

#include "config.h"
#include "color.h"
#include "cam.h"
#include "hittable.h"

namespace raze
{
    class Renderer {
    public:
        Renderer(const Config& config);
        void render(const Hittable& world);
        bool saveToPPM(const std::string& name) const;

    private:
        void renderRegion(const Hittable& world, std::vector<Color>& result, size_t start, size_t end);

        Vector3f rayColor(const raze::Ray& ray, int depth, const raze::Hittable& world) const;
        Vector3f sampleSquare() const;
        Ray getRay(int i, int j) const;
        Vector3f defocusDiscSample() const;

    private:
        Config    _Config;
        Cam       _Camera;
        int       _ImageWidth;
        int       _ImageHeight;
        float     _PixelSampleScale;

        std::vector<Color>  _FrameBuffer;
    };
}

#endif // RENDERER_H