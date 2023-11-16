#ifndef SYSTEM_H
#define SYSTEM_H


class System
{
public:
    virtual ~System() = default;
    virtual void Update(float deltaSeconds) = 0;
};

class DrawingSystem
{
public:
    virtual ~DrawingSystem() = default;
    virtual void Draw() = 0;
};



#endif //SYSTEM_H
