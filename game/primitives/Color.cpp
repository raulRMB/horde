#include "Color.h"


namespace tX
{

FColor::FColor(const u32 rgba)
{
    RGBA = rgba;
}

FColor::FColor(u8 r, u8 g, u8 b, u8 a)
{
    R = r;
    G = g;
    B = b;
    A = a;
}

FColor FColor::LerpRGBA(const FColor a, const FColor b, const f32 t)
{
    return FColor{
        (u8)((f32)a.R + (f32)(b.R - a.R) * t),
        (u8)((f32)a.G + (f32)(b.G - a.G) * t),
        (u8)((f32)a.B + (f32)(b.B - a.B) * t),
        (u8)((f32)a.A + (f32)(b.A - a.A) * t)
    };
}

}
