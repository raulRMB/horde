#ifndef ELEMENT_H
#define ELEMENT_H

class Canvas;

class Element
{
public:
    Canvas* canvas;
    virtual ~Element() = default;
    virtual bool isHovered() = 0;
    virtual void* OnDrag() = 0;
    virtual void OnDragCancelled() = 0;
    virtual void OnDrop(Element* source, void* payload) = 0;
    virtual void Draw() = 0;
    virtual void Update() = 0;
};

#endif