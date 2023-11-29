#ifndef HORDE_ANIMATION_COMPONENT_H
#define HORDE_ANIMATION_COMPONENT_H

struct CAnimation
{
    int CurrentFrame;
    int AnimsCount;
    int AnimsIndex;
    bool bPlaying;
    float CurrentFrameTime;
    float FrameTime;
    struct ModelAnimation* Animations;
};

#endif //HORDE_ANIMATION_COMPONENT_H
