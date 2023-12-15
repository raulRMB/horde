//
// Created by Raul on 11/17/2023.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "systems/System.h"

namespace tZ
{

class SPhysics : public UpdateSystem
{
    std::set<entt::entity> ToDestroy;
public:
    void Update(float deltaSeconds) override;
};

}

#endif //PHYSICSSYSTEM_H
