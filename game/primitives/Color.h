#ifndef F_COLOR_H
#define F_COLOR_H

#include "util/defines.h"

namespace tX
{

typedef struct FColor
{
    union
    {
        struct
        {
            u8 A;
            u8 B;
            u8 G;
            u8 R;
        };
        u32 RGBA;
    };

    FColor() : RGBA(0xFF35C5FF) {}
    FColor(u32 rgba);
    FColor(u8 r, u8 g, u8 b, u8 a);

    static FColor LerpRGBA(FColor a, FColor b, f32 t);
} FColor;



}

#endif //F_COLOR_H
