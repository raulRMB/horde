#ifndef HORDE_C_MORTAL_H
#define HORDE_C_MORTAL_H

#include "TechXComponent.h"
#include "util/defines.h"

namespace tZ
{

struct CMortal : CComponent
{
    bool bIsAlive;
    CMortal() : bIsAlive(true) {}
};

}


#endif //HORDE_C_MORTAL_H
