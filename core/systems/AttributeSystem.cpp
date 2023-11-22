#include "AttributeSystem.h"
#include <entt/entt.hpp>
#include "../components/Components.h"
#include "../../game/Game.h"
#include "../util/Util.h"
#include <algorithm>

void AttributeSystem::Init()
{
    Game::GetDispatcher().sink<Effect>().connect<&AttributeSystem::OnEffect>();
}

void AttributeSystem::Update(float deltaSeconds)
{
}

void AttributeSystem::OnEffect(const Effect &effect) {
    AttributesComponent& ac = Game::GetRegistry().get<AttributesComponent>(effect.target);
    Util::ApplyEffect(ac, effect);
}
