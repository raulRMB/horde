#include "Color.h"


namespace tZ
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

}
