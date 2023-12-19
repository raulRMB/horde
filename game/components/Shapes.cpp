#include "Shapes.h"

namespace tZ
{

CCube::CCube(const v3 &position, const v3 &size, const FColor &color)
    : Position(position), Size(size), Color(color), CComponent("Cube")
{
}


CMoveCircle::CMoveCircle(const v3 &position, float radius)
    : Position(position), Radius(radius), CComponent("MoveCircle")
{
}

CCapsule::CCapsule(const v3 &position, float radius, float height, int slices, const FColor &color)
    : Position(position), Radius(radius), Height(height), Slices(slices), Color(color), CComponent("Capsule")
{
}

CSphere::CSphere(const v3 &position, float radius, const FColor &color)
    : Position(position), Radius(radius), Color(color), CComponent("Sphere")
{
}


CLineStrip::CLineStrip(const std::vector<v3> &points, const FColor &color)
    : Points(points), Color(color), CComponent("LineStrip")
{
}

} // namespace tZ