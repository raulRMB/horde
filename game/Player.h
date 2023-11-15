#ifndef PLAYER_H
#define PLAYER_H

#include "SmartEntity.h"

class Player final : public SmartEntity
{
public:
    Player();
    ~Player();

    void Start() override;

    void HandleInput();

    void Kill() override;
};

#endif //PLAYER_H
