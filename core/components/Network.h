#ifndef HORDE_NETWORK_H
#define HORDE_NETWORK_H

#include <set>
#include <string>
#include "components/Transform.h"

namespace tZ
{

struct CNetwork {
    std::set<std::string> Tags;
    CTransform PrevTransform = CTransform{};
    CTransform TargetTransform = CTransform{};
};

}
#endif
