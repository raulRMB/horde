#ifndef HORDE_FOLLOW_COMPONENT_H
#define HORDE_FOLLOW_COMPONENT_H

#include <vector>
#include "components/tX_Component.h"

namespace tX
{

enum class EFollowState : unsigned int
{
    Idle = 0,
    Following,
    Dirty,
};

struct CFollow : CComponent
{
    std::vector<v2> StringPath;
    v2 TargetPos;
    unsigned int Index;
    v2 Goal;
    EFollowState FollowState;

    CFollow() : StringPath({}), TargetPos({0.0f, 0.0f}), Index(0), Goal({0.0f, 0.0f}),
    FollowState(EFollowState::Idle), CComponent("Follow") {}
};

struct CTarget : CComponent
{
    v3 Position;

    CTarget() : Position({0.0f, 0.0f, 0.0f}), CComponent("Target") {}
};

}

#endif //HORDE_FOLLOW_COMPONENT_H
