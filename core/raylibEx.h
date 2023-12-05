#ifndef RAYLIBEX_H
#define RAYLIBEX_H

#include "defines.h"
#include "primitives/Color.h"
#include "components/Rectangle.h"
#include "components/Transform.h"
#include <raylib.h>

namespace tZ
{
    // inline Rectangle operator=(const CRectangle& rect)
    // {
    //     return (Rectangle){rect.X, rect.Y, rect.Width, rect.Height};
    // }
    //
    // inline Rectangle operator Rectangle(const CRectangle& rect)
    // {
    //     return (Rectangle){rect.X, rect.Y, rect.Width, rect.Height};
    // }
    //
    // inline Color operator Color(const CColor& color)
    // {
    //     return (Color){color.R, color.G, color.B, color.A};
    // }
    //
    // inline Color operator=(const CColor& color)
    // {
    //     return (Color){color.R, color.G, color.B, color.A};
    // }

    inline Vector3 ToRaylibVector3(const v3& vec)
    {
        return (Vector3){vec.x, vec.y, vec.z};
    }

    inline Vector2 ToRaylibVector2(const v2& vec)
    {
        return (Vector2){vec.x, vec.x};
    }

    inline Rectangle ToRaylibRect(const CRectangle& rect)
    {
        return (Rectangle){rect.X, rect.Y, rect.Width, rect.Height};
    }

    inline Color ToRaylibColor(const FColor& color)
    {
        return (Color){color.R, color.G, color.B, color.A};
    }

    v2 GetMousePositionGLM();

}


#endif //RAYLIBEX_H
