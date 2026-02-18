#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vector3.h"

namespace raze
{
    class Color {
    public:
        Color(int value);
        Color(int r, int g, int b);
        Color(float rv, float gv, float bv);

        friend std::ostream& operator<<(std::ostream& out, const Color& c);

    public:
        int r, g, b;
    };

    float linearToGamma(float linearComponent);
}

#endif // COLOR_H