#ifndef PLAYER_H
#define PLAYER_H

#include "SmartEntity.h"

class Player final : public SmartEntity
{
public:
    Player();
    ~Player();
    Font font = LoadFont("../assets/ui/fonts/Roboto-Bold.ttf");
    void Start() override;

    void HandleInput();
    void DrawUI();

    void Kill() override;
};

#endif //PLAYER_H
