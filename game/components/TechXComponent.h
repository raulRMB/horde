#ifndef HORDE_C_TECHCOMPONENT_H
#define HORDE_C_TECHCOMPONENT_H

#include <utility>

namespace tZ
{

struct CComponent
{
    bool bNeedsNetSync;
    const char* TypeId;

    CComponent(const char* typeId) : bNeedsNetSync(false), TypeId(typeId) {}
};


}


#endif //HORDE_CTECHCOMPONENT_H
