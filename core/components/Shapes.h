//
// Created by raul on 28/11/23.
//

#ifndef HORDE_SHAPES_H
#define HORDE_SHAPES_H

#include <raylib.h>
#include <vector>

struct CCube
{
    Vector3 Position;
    Vector3 Size;
    Color color;
};

struct CCapsule
{
    Vector3 Position;
    float Radius;
    float Height;
    int Slices;
    Color color;
};

struct CSphere
{
    Vector3 Position;
    float Radius;
    Color color;
};

struct CRay
{
    Ray ray;
    Color color;
};

struct CTriangle
{
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Color color;
};

struct CLineStrip
{
    std::vector<Vector3> Points;
    Color color;
};

#endif //HORDE_SHAPES_H
