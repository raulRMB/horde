//
// Created by raul on 28/11/23.
//

#ifndef HORDE_SHAPES_H
#define HORDE_SHAPES_H

#include <vector>

namespace tZ
{

struct CCube
{
    v3 Position;
    v3 Size;
    CColor Color;
};

struct CCapsule
{
    v3 Position;
    float Radius;
    float Height;
    int Slices;
    CColor Color;
};

struct CSphere
{
    v3 Position;
    float Radius;
    CColor Color;
};

struct CTriangle
{
    v3 V1;
    v3 V2;
    v3 V3;
    CColor Color;
};

struct CLineStrip
{
    std::vector<v3> Points;
    CColor Color;
};

}

#endif //HORDE_SHAPES_H
