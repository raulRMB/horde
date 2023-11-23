//
// Created by Raul on 11/17/2023.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "systems/System.h"

class PhysicsSystem : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};

#endif //PHYSICSSYSTEM_H
