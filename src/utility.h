#ifndef UTILITY_H
#define UTILITY_H

#include "constants.h"

namespace raze 
{
    inline float degToRad(float degrees)
    {   
        return degrees * PI / 180.f;
    }

    inline Vector3f vectorFromColor(int r, int g, int b)
    {
        return Vector3f((float)r / 255.f, (float)g / 255.f, (float)b / 255.f);
    }
}

#endif // UTILITY_H