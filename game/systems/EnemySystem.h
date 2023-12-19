#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "systems/System.h"

namespace tZ
{

class EnemySystem : public UpdateSystem
{
    entt::entity Player;
public:
    void SetPlayer(entt::entity player) { Player = player; }
    void Update(float deltaSeconds) override;
    void Process(entt::entity e, entt::registry& r, float deltaSeconds);
};

}

#endif //ENEMY_SYSTEM_H