#ifndef C_COLOR_H
#define C_COLOR_H

#include "../defines.h"

namespace tZ
{

typedef struct FColor
{
    union
    {
        u8 R;
        u8 G;
        u8 B;
        u8 A;
        u32 RGBA;
    };

    FColor() = default;
    explicit FColor(u32 rgba);
    FColor(u8 r, u8 g, u8 b, u8 a);
} CColor;

static FColor White = FColor(0xFFFFFFFF);
static FColor Black = FColor(0x000000FF);
static FColor Red = FColor(0xFF0000FF);
static FColor Green = FColor(0x00FF00FF);
static FColor Blue = FColor(0x0000FFFF);
static FColor Cyan = FColor(0x00FFFFFF);
static FColor Violet = FColor(0xFF00FFFF);
static FColor Yellow = FColor(0xFFFF00FF);

}

#endif //C_COLOR_H
