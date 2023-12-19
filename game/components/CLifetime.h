#ifndef C_LIFETIME_H
#define C_LIFETIME_H

#include "components/tX_Component.h"
#include "util/defines.h"

namespace tX
{

struct CLifetime : CComponent
{
    f32 MaxLifetime;
    f32 CurrentLifetime;

    CLifetime(f32 maxLifetime = -1.f) : MaxLifetime(maxLifetime), CurrentLifetime(0.0f), CComponent("Lifetime") {}
};

}

#endif //C_LIFETIME_H
