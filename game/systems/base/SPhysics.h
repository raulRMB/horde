#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "systems/System.h"

namespace tX
{

class SPhysics : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
    void Process2D(entt::entity e, entt::registry& r, float deltaSeconds);
};

}

#endif //PHYSICSSYSTEM_H
