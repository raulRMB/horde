#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "systems/System.h"
#include "components/Components.h"

class AnimationSystem final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};

#endif //ANIMATIONSYSTEM_H
