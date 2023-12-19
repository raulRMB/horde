#ifndef F_RAY_H
#define F_RAY_H

#include "util/defines.h"

namespace tX
{

struct FRay
{
    v3 Origin;
    v3 Direction;

    FRay() = default;
};

}

#endif //F_RAY_H
