#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
public:
    virtual ~Element() = default;
    virtual bool isHovered() = 0;
    virtual void Draw() = 0;
    virtual void Update() = 0;
};

#endif