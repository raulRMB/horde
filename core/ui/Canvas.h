#ifndef CANVAS_H
#define CANVAS_H

#include "Element.h"
#include <vector>
#include <any>

class Canvas {
    Vector2 screenSize;
    std::vector<Element*> elements;
    bool activeClick = false;
    bool activeDrag = false;
    Vector2 activeDragStartPos;
    Element* dragged;
    std::any draggedPayload;
    Texture2D dragImage;
public:
    Canvas();
    void Add(Element* element);
    void Draw();
    void Update();
private:
    Element* FindFirstHoveredElement();
    void HandleEvents();
};

#endif