#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
public:
    virtual ~Element() = default;
    virtual bool isHovered();
    virtual void Draw();
    virtual void Update();
};

#endif