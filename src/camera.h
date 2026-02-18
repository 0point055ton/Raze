#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "config.h"
#include "hittable.h"
#include "random.h"
#include "utility.h"

namespace raze
{
    class Camera {
    public:
        Camera(Config config);
        void render(const Hittable& world);
        bool saveToPPM(const std::string& name) const;
        
    private:
        void initialize();
        Vector3f rayColor(const raze::Ray& ray, int depth, const raze::Hittable& world);
        Vector3f sampleSquare();
        Ray getRay(int i, int j);
        Vector3f defocusDiscSample();

    private:
        Config      _Config;
        Vector3f    _Center;
        Vector3f    _LookFrom;
        Vector3f    _LookAt;
        Vector3f    _VUp;
        Vector3f    _U, _V, _W;
        Vector3f    _DefocusDiscU;
        Vector3f    _DefocusDiscV;
        Vector3f    _PixelDeltaU;
        Vector3f    _PixelDeltaV;
        Vector3f    _Pixel00Loc;
        int         _ImageWidth;
        int         _ImageHeight;
        float       _PixelSampleScale;

        std::vector<Color> _FrameBuffer;
    };
}

#endif // CAMERA_H