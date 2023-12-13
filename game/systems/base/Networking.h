#ifndef HORDE_SNETWORKING_H
#define HORDE_SNETWORKING_H

#include "systems/System.h"

namespace tZ
{

class SNetworking : public UpdateSystem {
public:
    void Update(float deltaSeconds) override;
};

}

#endif
