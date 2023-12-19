#ifndef HORDE_CSHAPES_H
#define HORDE_CSHAPES_H

#include <vector>
#include "components/tX_Component.h"
#include "util/defines.h"
#include "primitives/Color.h"

namespace tX
{

struct CCube : CComponent
{
    v3 Position;
    v3 Size;
    FColor Color;

    CCube(const v3& position = v3(0.f), const v3& size = v3(1.f), const FColor& color = FColor(0x000000ff));
};

struct CMoveCircle : CComponent
{
    v3 Position;
    float Radius;

    CMoveCircle(const v3& position = v3(0.f), float radius = 1.f);
};

struct CCapsule : CComponent
{
    v3 Position;
    float Radius;
    float Height;
    int Slices;
    FColor Color;

    CCapsule(const v3& position = v3(0.f), float radius = 1.f, float height = 1.f, int slices = 16, const FColor& color = FColor(0x000000ff));
};

struct CSphere : CComponent
{
    v3 Position;
    float Radius;
    FColor Color;

    CSphere(const v3& position = v3(0.f), float radius = 1.f, const FColor& color = FColor(0x000000ff));
};

struct CTriangle : CComponent
{
    v3 V1;
    v3 V2;
    v3 V3;
    FColor Color;

    CTriangle(const v3& vec1 = v3(0.f), const v3& vec2 = v3(0.f), const v3& vec3 = v3(0.f), const FColor& color = FColor(0x000000ff));
};

struct CLineStrip : CComponent
{
    std::vector<v3> Points;
    FColor Color;

    CLineStrip(const std::vector<v3>& points = std::vector<v3>(), const FColor& color = FColor(0x000000ff));
};

}

#endif //HORDE_CSHAPES_H
