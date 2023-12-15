#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "systems/System.h"

namespace tZ
{

class SEnemy : public UpdateSystem
{
    entt::entity Player;
public:
    void SetPlayer(entt::entity player) { Player = player; }
    void Update(float deltaSeconds) override;
}; 

}

#endif //ENEMY_SYSTEM_H