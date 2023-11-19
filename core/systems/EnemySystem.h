#ifndef ENEMYSYSTEM_H
#define ENEMYSYSTEM_H

#include "System.h"

class EnemySystem final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};



#endif //ENEMYSYSTEM_H
