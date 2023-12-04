#ifndef HORDE_ANIMATION_COMPONENT_H
#define HORDE_ANIMATION_COMPONENT_H

#include "raylib.h"

namespace tZ
{

struct CAnimation
{
    int CurrentFrame;
    int AnimsCount;
    int AnimsIndex;
    bool bPlaying;
    float CurrentFrameTime;
    float FrameTime;
    ModelAnimation* Animations;
};
    
}
#endif //HORDE_ANIMATION_COMPONENT_H
