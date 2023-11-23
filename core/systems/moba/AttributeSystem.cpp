#include "AttributeSystem.h"
#include "entt/entt.hpp"
#include "components/Components.h"
#include "Game.h"
#include "util/Util.h"
#include <algorithm>

void AttributeSystem::Init()
{
    Game::GetDispatcher().sink<Effect>().connect<&AttributeSystem::OnEffect>();
}

void AttributeSystem::Update(float deltaSeconds)
{
}

void AttributeSystem::OnEffect(const Effect &effect) {
    AttributesComponent& target = Game::GetRegistry().get<AttributesComponent>(effect.target);
    AttributesComponent& source = Game::GetRegistry().get<AttributesComponent>(effect.source);
    if(effect.type == INSTANT) {
        effect.callback(target, source);
    }
}
