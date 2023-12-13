#ifndef C_LIFETIME_H
#define C_LIFETIME_H

#include "components/TechXComponent.h"
#include "util/defines.h"

namespace tZ
{

struct CLifetime : CComponent
{
    f32 MaxLifetime = 0.0f;
    f32 CurrentLifetime = 0.0f;
};

}

#endif //C_LIFETIME_H
