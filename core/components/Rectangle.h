#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "defines.h"

namespace tZ
{

struct CRectangle
{
    union
    {
        v2 Position;
        float X;
        float Y;
    };

    union
    {
        v2 Dimensions;
        float Width;
        float Height;
    };

    CRectangle() = default;
    CRectangle(v2 position, v2 dimensions);
};

}

#endif //RECTANGLE_H
