//
// Created by Raul Romero on 2023-11-18.
//

#ifndef HORDE_FOLLOW_COMPONENT_H
#define HORDE_FOLLOW_COMPONENT_H

#include <raylib.h>
#include <vector>

enum class EFollowState : unsigned int
{
    Idle = 0,
    Following,
    Dirty,
};

struct FollowComponent
{
    std::vector<Vector2> StringPath;
    Vector2 TargetPos;
    unsigned int Index;
    Vector2 Goal;
    EFollowState FollowState;
};

#endif //HORDE_FOLLOW_COMPONENT_H
