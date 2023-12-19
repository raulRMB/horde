#include "Spawner.h"

namespace tZ
{

CSpawner::CSpawner(float spawnRate) : SpawnRate(spawnRate), SpawnTimer(0.f), CComponent("Spawner")
{
}

} // namespace tZ