#include "SmartEntity.h"

namespace tZ
{

void SmartEntity::Kill()
{
    Game::GetRegistry().destroy(Entity);
}

}