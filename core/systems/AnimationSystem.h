#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "System.h"

class AnimationSystem final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};



#endif //ANIMATIONSYSTEM_H
