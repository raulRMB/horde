#ifndef HORDE_S_MORTAL_H
#define HORDE_S_MORTAL_H

#include "systems/System.h"

namespace tZ
{

class SMortal : public System
{
public:
    void OnHealthChanged(entt::entity entity, f32 newHealth);
    static void Kill(entt::entity entity);
};

}

#endif //HORDE_S_MORTAL_H
