#ifndef CANVAS_H
#define CANVAS_H

#include "Element.h"
#include <vector>
#include <any>
#include "util/defines.h"

namespace tZ
{

class Canvas
{
    v2 screenSize;
    std::vector<Element*> elements;
    bool activeClick = false;
    bool activeDrag = false;
    v2 activeDragStartPos;
    Element* dragged;
    std::any draggedPayload;
public:
    Canvas();
    void Add(Element* element);
    void Draw();
    void Update();
private:
    Element* FindFirstHoveredElement();
    void HandleEvents();
};

}

#endif