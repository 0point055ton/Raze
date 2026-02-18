#include "color.h"
#include "interval.h"

namespace raze 
{
    Color::Color(int value) : r(value), g(value), b(value) {}
    Color::Color(int r, int g, int b) : r(r), g(g), b(b) {}

    Color::Color(float rv, float gv, float bv)
    {
        rv = linearToGamma(rv);
        gv = linearToGamma(gv);
        bv = linearToGamma(bv);

        static const Interval<float> intensity(0.f, 0.999f);
        r = int(256 * intensity.clamp(rv));
        g = int(256 * intensity.clamp(gv));
        b = int(256 * intensity.clamp(bv));
    }

    std::ostream& operator<<(std::ostream& out, const Color& c)
    {
        return out << c.r << ' ' << c.g << ' ' << c.b;
    }

    float linearToGamma(float linearComponent)
    {
        if (linearComponent > 0.f)
            return std::sqrtf(linearComponent);

        return 0.f;
    }
}