#ifndef HORDE_CNETWORK_H
#define HORDE_CNETWORK_H

#include <set>
#include <string>
#include "components/tX_Component.h"

namespace tX
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
