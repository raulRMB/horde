#ifndef HORDE_SCHARACTERANIMATION_H
#define HORDE_SCHARACTERANIMATION_H

#include "systems/System.h"

namespace tZ
{
struct CCharacterAnimation;
struct CPhysics2D;

class SCharacterAnimation : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
    void ResetAttackAnimation(CCharacterAnimation &characterAnim);
    void DecideBaseAnimation(CCharacterAnimation &characterAnim, CPhysics2D& physics);
    void DecideOverrideAnimation(f32 deltaSeconds, CCharacterAnimation& characterAnim);
};

}

#endif //HORDE_SCHARACTERANIMATION_H
