//
// Created by Raul Romero on 2023-12-14.
//

#include "Mortal.h"

namespace tZ
{

void SMortal::Kill(const entt::entity entity)
{
    DestroyEntity(entity);
}

void SMortal::OnHealthChanged(entt::entity entity, const f32 newHealth)
{
    if(newHealth <= 0.0f)
        Kill(entity);
}

}
