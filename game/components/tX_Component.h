#ifndef HORDE_C_TECHXCOMPONENT_H
#define HORDE_C_TECHXCOMPONENT_H

#include <utility>

namespace tX
{

struct CComponent
{
    bool bNeedsNetSync;
    const char* TypeId;

    CComponent(const char* typeId) : bNeedsNetSync(false), TypeId(typeId) {}
};


}


#endif //HORDE_C_TECHXCOMPONENT_H
