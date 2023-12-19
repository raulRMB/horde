#include "CRay.h"

namespace tX
{

CRay::CRay(const FRay& ray, FColor color)
    : Ray(ray), Color(color), CComponent("Ray")
{

}

}