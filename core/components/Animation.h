#ifndef HORDE_ANIMATION_COMPONENT_H
#define HORDE_ANIMATION_COMPONENT_H

namespace raylib
{
#include "raylib.h"
}
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
    raylib::ModelAnimation* Animations;
};
    
}
#endif //HORDE_ANIMATION_COMPONENT_H
