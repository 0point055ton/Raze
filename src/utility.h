#ifndef UTILITY_H
#define UTILITY_H

#include "constants.h"

namespace raze 
{
    inline float degToRad(float degrees)
    {   
        return degrees * PI / 180.f;
    }
}

#endif // UTILITY_H