#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "systems/System.h"

namespace tX
{

class SAnimation final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};

}

#endif //ANIMATIONSYSTEM_H
