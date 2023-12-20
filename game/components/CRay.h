#ifndef C_RAY_H
#define C_RAY_H

#include "primitives/Color.h"
#include "primitives/Ray.h"
#include "components/tX_Component.h"

namespace tX
{

struct CRay : CComponent
{
    FRay Ray;
    FColor Color;

    CRay(const FRay& ray = FRay(), FColor color = FColor(0x000000ff));
};

}

#endif //C_RAY_H