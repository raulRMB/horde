#include "Ray.h"

namespace tZ
{

CRay::CRay(const FRay& ray, FColor color)
    : Ray(ray), Color(color), CComponent("Ray")
{

}

}