#ifndef ELEMENT_H
#define ELEMENT_H

#include <any>
#include "util/defines.h"

namespace tZ
{

class Canvas;

struct DrawData {
    bool activeDrag;
    v2 screenSize;
};

class Element
{
public:
    char* name;
    Canvas* canvas;
    virtual ~Element() = default;
    virtual bool isHovered() = 0;
    virtual std::any OnDrag() = 0;
    virtual void OnHover() = 0;
    virtual void OnHoverExit() = 0;
    virtual void OnDragCancelled() = 0;
    virtual void OnDrop(Element* source, std::any payload) = 0;
    virtual void OnAdded() = 0;
    virtual void Draw(DrawData data) = 0;
    virtual void Update() = 0;
    virtual void OnWindowResize(v2 screenSize) = 0;
    bool hoverActive = false;
};

}

#endif