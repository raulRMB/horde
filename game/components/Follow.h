#ifndef HORDE_FOLLOW_COMPONENT_H
#define HORDE_FOLLOW_COMPONENT_H

#include <vector>

namespace tZ
{

enum class EFollowState : unsigned int
{
    Idle = 0,
    Following,
    Dirty,
};

struct CFollow
{
    std::vector<v2> StringPath;
    v2 TargetPos;
    unsigned int Index;
    v2 Goal;
    EFollowState FollowState;
};

struct CTarget
{
    v3 Position;
};

}

#endif //HORDE_FOLLOW_COMPONENT_H
