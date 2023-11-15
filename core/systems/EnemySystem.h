#ifndef ENEMYSYSTEM_H
#define ENEMYSYSTEM_H

#include "System.h"

class EnemySystem final : public System
{
public:
    void Update(float deltaSeconds) override;
};



#endif //ENEMYSYSTEM_H
