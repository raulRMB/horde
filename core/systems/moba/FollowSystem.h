#ifndef FOLLOWSYSTEM_H
#define FOLLOWSYSTEM_H

#include "systems/System.h"

class FollowSystem final : public UpdateSystem
{


public:
    void Update(float deltaSeconds);
};



#endif //FOLLOWSYSTEM_H