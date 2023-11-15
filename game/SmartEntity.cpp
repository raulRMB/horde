#include "SmartEntity.h"

void SmartEntity::Kill()
{
    Game::GetRegistry().destroy(Entity);
}
