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
    FColor Color;
};

struct CMoveCircle {
    v3 Position;
    float Radius;
};

struct CCapsule
{
    v3 Position;
    float Radius;
    float Height;
    int Slices;
    FColor Color;
};

struct CSphere
{
    v3 Position;
    float Radius;
    FColor Color;
};

struct CTriangle
{
    v3 V1;
    v3 V2;
    v3 V3;
    FColor Color;
};

struct CLineStrip
{
    std::vector<v3> Points;
    FColor Color;
};

}

#endif //HORDE_SHAPES_H
