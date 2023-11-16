#ifndef CANVAS_H
#define CANVAS_H

#include "Element.h"
#include <vector>

class Canvas {
    std::vector<Element*> elements;
    bool activeClick = false;
    bool activeDrag = false;
    Vector2 activeDragStartPos;
    Element* dragged;
    void* draggedPayload;
public:
    void Add(Element* element);
    void Draw();
    void Update();
private:
    Element* FindFirstHoveredElement();
    void HandleEvents();
};

#endif