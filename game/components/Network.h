#ifndef HORDE_NETWORK_H
#define HORDE_NETWORK_H

#include <set>
#include <string>
#include "components/TechXComponent.h"

namespace tZ
{

struct CNetwork : CComponent
{
    std::set<std::string> Tags;
    CTransform PrevTransform;
    CTransform TargetTransform;

    CNetwork() : Tags({}), PrevTransform(CTransform()), TargetTransform(CTransform()), CComponent("Network") {}
};

}
#endif
