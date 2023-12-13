#ifndef PLAYER_H
#define PLAYER_H

#include "SmartEntity.h"

namespace tZ
{

class Player final : public SmartEntity
{
public:
    Player();
    ~Player();
    void Start() override;

    void HandleInput(entt::registry* Registry);
    void DrawUI();
    void SetTransform(CTransform& t);

    void Kill() override;
};

}

#endif //PLAYER_H
