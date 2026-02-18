#ifndef CAM_H
#define CAM_H

#include "vector3.h"

namespace raze
{
    struct Cam
    {
        Vector3f _LookFrom;
        Vector3f _LookAt;
    };
}

#endif // CAM_H