#ifndef HORDE_ANIMATION_COMPONENT_H
#define HORDE_ANIMATION_COMPONENT_H

namespace tX
{

struct CAnimation
{
    int CurrentFrame;
    int AnimsCount;
    int AnimsIndex;
    bool bPlaying;
    float CurrentFrameTime;
    float FrameTime;
    //raylib::ModelAnimation* Animations;
    void Reset(int index) {
        AnimsIndex = index;
        CurrentFrame = 0;
        CurrentFrameTime = 0;
        FrameTime = 0;
    }
};
    
}
#endif //HORDE_ANIMATION_COMPONENT_H
