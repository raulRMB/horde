//
// Created by Raul on 11/17/2023.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class PhysicsSystem : public System
{
public:
    void Update(float deltaSeconds) override;
};

#endif //PHYSICSSYSTEM_H
