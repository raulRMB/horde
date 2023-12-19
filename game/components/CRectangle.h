#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "components/tX_Component.h"
#include "util/defines.h"

namespace tX
{

struct CRectangle : CComponent
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

    CRectangle(v2 position = v2(0.f), v2 dimensions = v2(1.f));
};

}

#endif //RECTANGLE_H
