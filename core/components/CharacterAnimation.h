#ifndef HORDE_CCHARACTERANIMATION_H
#define HORDE_CCHARACTERANIMATION_H

#include "defines.h"
#include "TechXComponent.h"

namespace tZ
{

enum class ECharacterAnimState : i32
{
    Attacking1 = 1,
    Dying = 2,
    Hit = 3,
    Idle = 4,
    Running = 5,
    Walking = 7,
    Dead,
    Attacking2,
    Attacking3,
    Attacking4
};

struct CCharacterAnimation : CComponent
{
    ECharacterAnimState AnimState;
    bool bOverrideAnim;
    f32 CurrentAnimTime;
    f32 EndAnimTime;
};

}

#endif //HORDE_CCHARACTERANIMATION_H
