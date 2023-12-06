#ifndef RAYLIBEX_H
#define RAYLIBEX_H

#include "defines.h"
#include "primitives/Color.h"
#include "components/Rectangle.h"
#include "components/Transform.h"
namespace raylib
{
    #include <raylib.h>
}

namespace tZ
{
    inline raylib::Vector3 ToRaylibVector3(const v3& vec)
    {
        return (raylib::Vector3){vec.x, vec.y, vec.z};
    }

    inline raylib::Vector2 ToRaylibVector2(const v2& vec)
    {
        return (raylib::Vector2){vec.x, vec.y};
    }

    inline raylib::Rectangle ToRaylibRect(const CRectangle& rect)
    {
        return (raylib::Rectangle){rect.X, rect.Y, rect.Width, rect.Height};
    }

    inline raylib::Color ToRaylibColor(const FColor& color)
    {
        return (raylib::Color){color.R, color.G, color.B, color.A};
    }

    v2 GetMousePositionGLM();

}


#endif //RAYLIBEX_H
